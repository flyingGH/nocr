set (exec_name icdar2013-evaluation)

add_executable( ${exec_name} main.cpp )

target_link_libraries( ${exec_name} NOCRLib )
target_link_libraries( ${exec_name} ${OpenCV_LIBS} )
target_link_libraries( ${exec_name} ${required_libraries})
