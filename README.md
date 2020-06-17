# faceDetectionTask

libraries are used:
opencv_4_2
boost_1_72_0

i used opencv/dnn for face detecting with files for configuring my dnn -- deploy.prototxt.txt and res10_300x300_ssd_iter_140000.caffemodel

# Installation in Linux:

paste opencv and opencv_contrib to the Project/facedetect directory 

build opencv via CMake 

build facedetect library via CMake : 

  cd Project
  
  cd facedetect
  
  sudo cmake .
  
  sudo make install
  
  
paste boost_1_72_0 to the Project directory 

build boost

then run

  sudo cmake .
  
  sudo make install
  
to run project go to Project directory, then in cmd type 

sudo ./app "<PathToDirectoryWithPhotos>" 

# installation in Windows

the same with boost and opencv , you need to build them and paste boost to Project directory and opencv to Project/facedetect directory 

then cd Project/facedetect 

  cmake -G "Visual Studio <version> <year>" .  (example "Visual Studio 16 2019")
  
  cmake --build . --target ALL_BUILD --config Release
  
and do the same in the ./Project directory
