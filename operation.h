#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <sstream>

class Operation {
public: 
	Operation(std::string line) {
		size_t startPos = 0;
		size_t endPos = 0; 
		std::vector<std::string> words; 
		while (endPos < line.length()-1) {
			endPos = line.find(" ", startPos);  
			std::string value = line.substr(startPos, endPos - startPos); 
			if (value.find("\"") != std::string::npos) {
				startPos++; 
				endPos = line.find("\"", startPos); 
				value = line.substr(startPos, endPos - startPos);
			}
			startPos = endPos + 1; 
			words.push_back(value); 
		}
		
		if (words.size() == 4) {
			action = words[0]; 
			comment_id = words[1]; 
			author = words[2]; 
			commentStr = words[3];
		}
		else if (words.size() == 5) {
			action = words[0]; 
			parent_comment_id = words[1];
			comment_id = words[2]; 
			author = words[3]; 
			commentStr = words[4];
		}
		else if (words.size() == 2) {
			action = words[0];
			comment_id = words[1]; 
		}
	}
	
	//accessors
	std::string getAction() {
		return action;
	}
	
	std::string getParentCommentId() {
		return parent_comment_id;
	}
	
	std::string getCommentId() {
		return comment_id;
	}
	
	std::string getAuthor() {
		return author;
	}
	
	std::string getComment() {
		return commentStr; 
	}
	
private:
	std::string action, parent_comment_id, comment_id, author, commentStr; 

};