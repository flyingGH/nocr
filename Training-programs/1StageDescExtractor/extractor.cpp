
#include <pugi/pugixml.hpp>
#include "extractor.hpp"
#include <algorithm>

void 
TruePositiveExtractor ::loadXml(const std::string & xml_file)
{
    std::ifstream ifs; 
    ifs.open(xml_file);

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load( ifs );

    pugi::xml_node root = doc.first_child();
    pugi::xml_node gt_train = root.first_child();

    for(pugi::xml_node img_node : gt_train.children("image"))
    {
        string img_name = getFileName(img_node.child("image-path").child_value());
        vector<LetterRecord> letters;
        auto gt = img_node.child("image-gt");

        for (pugi::xml_node word_node : gt.children("word"))
        {
            for (pugi::xml_node letter : word_node.children())
            {
                char c = letter.child_value()[0];
                int b = letter.attribute("b").as_int();
                int g = letter.attribute("g").as_int();
                int r = letter.attribute("r").as_int();

                cv::Point center;
                center.x = letter.attribute("center-x").as_int();
                center.y = letter.attribute("center-y").as_int();

                cv::Point tl, br;
                tl.x = letter.attribute("tl-x").as_int();
                tl.y = letter.attribute("tl-y").as_int();

                br.x = letter.attribute("br-x").as_int();
                br.y = letter.attribute("br-y").as_int();

                cv::Vec3b color;
                color[0] = b;
                color[1] = g;
                color[2] = r;

                cv::Rect rect(tl, br);
                letters.push_back({ color, rect, center, c});
            }
        }



        gt_records_.insert(std::make_pair(img_name, letters));
    }
}

void 
TruePositiveExtractor::setFileImage(const std::string & file_name)
{
    current_it_ = gt_records_.find(file_name);
}

void 
TruePositiveExtractor::operator()(ERRegion & err)
{
    auto gt_letters = current_it_->second;
    cv::Rect err_rect = err.getRectangle();

    auto it = std::find_if(gt_letters.begin(), gt_letters.end(),
            [this, &err_rect] (const LetterRecord & rec) -> bool
            { return areMatching(err_rect, rec.rect); });

    bool is_letter = it != gt_letters.end();

    if ((positive_ && is_letter) || (!positive_ && !is_letter))
    {
        char c = positive_ && is_letter ? it->letter : 'n';
        output_->save(err, current_it_->first, c);
    }
}


bool 
TruePositiveExtractor::areMatching(
        const cv::Rect & c_rect, 
        const cv::Rect & gt_rect)
{
    cv::Rect intersection = c_rect & gt_rect;

    if (intersection.area() == 0)
    {
        return false;
    }

    bool gt_condition = (double)intersection.area()/gt_rect.area() > 0.7;
    bool detected_condition = (double)intersection.area()/c_rect.area() > 0.4;

    return gt_condition && detected_condition;
}

void 
TruePositiveExtractor::notifyResize(double scale)
{
    std::for_each(current_it_->second.begin(), current_it_->second.end(), 
            [scale] (LetterRecord & rec)
            {
                cv::Point tl_rect = rec.rect.tl();
                cv::Point br_rect = rec.rect.br();

                cv::Rect scaled_rect = cv::Rect(scale * tl_rect,scale * br_rect);
                rec.rect = scaled_rect;
            });
}
