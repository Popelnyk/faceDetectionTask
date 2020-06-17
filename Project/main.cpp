#include "facedetect.h"

#include <iostream>
#include <string>
#include <vector>

#include <cstdlib>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace pt = boost::property_tree;

std::vector<std::string> getAllFilesInDir(const std::string& dirPath, const std::vector<std::string> dirSkipList = { })
{
 
    // Create a vector of string
    std::vector<std::string> listOfFiles;
    try {
        // Check if given path exists and points to a directory
        if (boost::filesystem::exists(dirPath) && boost::filesystem::is_directory(dirPath)) {
            // Create a Recursive Directory Iterator object and points to the starting of directory
            boost::filesystem::recursive_directory_iterator iter(dirPath);
 
            // Create a Recursive Directory Iterator object pointing to end.
            boost::filesystem::recursive_directory_iterator end;
 
            // Iterate till end
            while (iter != end) {
                // Check if current entry is a directory and if exists in skip list
                if (boost::filesystem::is_directory(iter->path()) && 
                (std::find(dirSkipList.begin(), dirSkipList.end(), iter->path().filename()) != dirSkipList.end())) {
                    // Skip the iteration of current directory pointed by iterator
                    iter.no_push();
                }
                else {
                    // Add the name in vector
                    listOfFiles.push_back(iter->path().string());
                }
 
                boost::system::error_code ec;
                // Increment the iterator to point to next entry in recursive iteration
                iter.increment(ec);
                if (ec) {
                    std::cerr << "Error While Accessing : " << iter->path().string() << " :: " << ec.message() << '\n';
                }
            }
        }
    }
    catch (std::system_error& e) {
        std::cerr << "Exception :: " << e.what();
    }
    
    return listOfFiles;
}


int main(int argc, char* argv[]) {
    
    if (argc < 2)
    {
        std::cout << "Usage: ./app path_to_directory_with_photos\n";
        return 1;
    }

    auto files = getAllFilesInDir(argv[1]);
    std::string mainPath(argv[1]);
    pt::ptree mainRoot;
    
    for(auto iter = files.begin(); iter != files.end(); ++iter) {
        auto coordinates = detect(*iter);
        pt::ptree root;
        std::string photoName = "";
        std::string fullPathToPhoto(*iter);
        
        //get current photo name
        for(int i = fullPathToPhoto.length() - 1; i >= 0; --i) {
            if(fullPathToPhoto[i] == '/') 
                break;
            photoName += fullPathToPhoto[i];
        }
        
        std::reverse(photoName.begin(), photoName.end());
        
        
        pt::ptree allCoordinatesNode;
        
        //get coordinates of each face detected on photo and prepare it for placing in json
        for(auto itercrd = coordinates.begin(); itercrd != coordinates.end(); ++itercrd) {
            pt::ptree coordinates;
            coordinates.put("x1", std::get<0>(*itercrd));
            coordinates.put("y1", std::get<1>(*itercrd));
            coordinates.put("x2", std::get<2>(*itercrd));
            coordinates.put("y2", std::get<3>(*itercrd));
            allCoordinatesNode.push_back(std::make_pair("", coordinates));
	}
	
	root.push_back(std::make_pair("coordinates", allCoordinatesNode));
	mainRoot.push_back(std::make_pair(photoName, root));
    }
    
    pt::write_json(mainPath + "/result.json", mainRoot);
	
	
    return 0;
}
