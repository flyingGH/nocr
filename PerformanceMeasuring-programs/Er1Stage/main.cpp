

#include <iostream>
#include <chrono>
#include <string>
#include <vector>

#include <nocrlib/extremal_region.h>
#include <nocrlib/component_tree_builder.h>
#include <nocrlib/component.h>
#include <nocrlib/utilities.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp> 

#include "measure_task.hpp"


void buildTree(ERTree & er_tree, 
        ComponentTreeBuilder<ERTree> & builder)
{
    builder.buildTree();
    er_tree.transformExtreme();
    er_tree.rejectSimilar();
}

using namespace std;

int main(int argc, char ** argv)
{
    string line;
    ErTreeBuildTask task;
    
    vector<cv::Mat> images;

    while (getline(cin, line))
    {
        cout << line << endl;
        cv::Mat image = cv::imread(line, CV_LOAD_IMAGE_GRAYSCALE);
        images.push_back(image);
    }

    MeasureTask<ErTreeBuildTask, vector<cv::Mat>, 1> measuring;
    cout << measuring.measureWallClockTime(task, images) 
        << " ms" << endl;


    /*
     * ERTree er_tree(min_area_ratio, max_area_ratio);
     * er_tree.setMinGlobalProbability(0.2);
     * er_tree.setMinDifference(0.1);

     * std::unique_ptr<ERFilter1Stage> er_function( new ERFilter1Stage() );
     * er_function->loadConfiguration(er1_conf_file);

     * er_tree.setERFunction(std::move(er_function));
     * ComponentTreeBuilder<ERTree> builder( &er_tree );
     */
    
    /*
     * uUINT64 start = getRelativeTime();
     * while (getline(cin, line)) 
     * {
     *     cv::Mat image = cv::imread(line, CV_LOAD_IMAGE_GRAYSCALE);
     *     string file_name = getFileName(line);

     *     cout << line << endl;

     *     er_tree.setImage(image);
     *     buildTree( er_tree, builder );
     *     auto components = er_tree.toComponent();
     *     er_tree.invertDomain();
     *     er_tree.deallocateTree();
     *     buildTree( er_tree, builder );
     *     auto tmp = er_tree.toComponent();
     *     components.insert(components.end(), tmp.begin(), tmp.end());
     * }

     * uUINT64 end = getRelativeTime();

     * cout << "length of run: " << end - start << "ms" << endl;
     */
    
    return 0;
}
