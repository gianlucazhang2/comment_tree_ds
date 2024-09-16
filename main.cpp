#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include "comment.h"
#include "operation.h"

//vector that stores all the comments of a video
std::vector<Comment> comments; 
//vector that stores all the operations 
std::vector<Operation> operations;  

//reference variables: video_id, video_owner
std::string video_owner, video_id; 

void readJSON(std::string json_file) {
	std::ifstream jsonFile(json_file);
    if (!jsonFile.is_open()) {
            std::cerr << "Failed to open the JSON file." << std::endl;
            exit(1);
    }
    std::string line;
    while (std::getline(jsonFile, line)) {
        Comment c(line); 
		comments.push_back(c); 
		//reference variables: video_id, video_owner
		if (c.getIsVideoOwner()) {
			video_owner = c.getAuthor();
		}
		video_id = c.getVideoId();
    }
    jsonFile.close();
}

void readInput(std::string input_file) {
	std::string line;
	std::ifstream input_str(input_file);
	while (std::getline(input_str, line)) { 
		Operation o(line); 
		operations.push_back(o);
	}
	input_str.close(); 
}

void deleteComment(std::string id) {
    // Create a new vector to store comments to keep
    std::vector<Comment> updatedComments;
	std::string parentComment; 
	for (Comment c: comments) {
		if (c.getCommentId() == id) {
			parentComment = c.getParentCommentId();
		}
	}
    for (Comment& c : comments) {
        if (c.getCommentId() == id) {
            continue;
        }
        if (c.getParentCommentId() == id) {
            deleteComment(c.getCommentId());
        }
		if (c.getCommentId() == parentComment) {
			c.setReplyCount(0); 
		}
        updatedComments.push_back(c);
    }
    comments = updatedComments;
	
}

//tree data structure
template <typename T>
class TreeNode {
public:
	T& data;
	int depth; 
	std::vector<TreeNode<T>*> replies; 
	TreeNode(T& comment) : data(comment) {}
	//destructor
	~TreeNode() {
		for (TreeNode<T>* reply : replies) {
			delete reply; 
		}
	}
};

void buildTree(TreeNode<Comment>* parentNode, Comment& parentComment) {
	for (Comment& c: comments) {
		if (c.getParentCommentId() == parentComment.getCommentId()) {
			TreeNode<Comment>* replyNode = new TreeNode<Comment>(c);
			replyNode->depth = parentNode->depth + 1;  
			parentNode->replies.push_back(replyNode); 
			buildTree(replyNode, c); 
		}
	}
}

//print out the nested comments 
void printTree(TreeNode<Comment>* node, std::ofstream& out) {
	if (node) {
		std::string indent = ""; 
		if (node->depth > 0) {
			for (int i = 0; i < node->depth; i++) {
				indent +=  "    "; 
			}
		}
		//print out comment
		out << indent << node->data.getAuthor() << " " << node->data.getPublishedDate() << std::endl; 
		out << indent << node->data.getComment() << std::endl; 
		out << indent << "\U0001F44D " << node->data.getLikeCount() << std::endl; 
		if (node->data.getReplyCount() > 1) {
			out << indent << node->data.getReplyCount() << " replies" << std::endl;
		}
		else if (node->data.getReplyCount() == 1) {
			out << indent << node->data.getReplyCount() << " reply" << std::endl;
		}		
		for (TreeNode<Comment>* reply: node->replies) {
			printTree(reply, out); 
		}
	}
}

int main(int argc, char* argv[]) {
	//command (exe, json, input, output)
	if (argc != 4) {return 1;} 
	
	std::string json_file = argv[1]; 
	readJSON(json_file); 
	
	std::string input_file = argv[2]; 
	readInput(input_file); 
	
	std::string output_file = argv[3]; 
	std::ofstream outstream(output_file);
	
	for (Operation o: operations) { 
		if (o.getAction() == "reply_to_video") {//reply to video case
			//check if author is the owner
			bool is_owner = false; 
			if (o.getAuthor() == video_owner) {is_owner = true;}
			Comment comment(video_id, o.getAuthor(), o.getCommentId(), 0, 0, false, "", "0 seconds ago", "2023-10-29T21:26:39.024635",
			is_owner, o.getComment()); 
			comments.push_back(comment); 
		}
		else if (o.getAction() == "reply_to_comment") {//reply to comment case
			//check if author is the owner
			bool is_owner = false; 
			if (o.getAuthor() == video_owner) {is_owner = true;}
			Comment comment(video_id, o.getAuthor(), o.getCommentId(), 0, 0, true, o.getParentCommentId(), "0 seconds ago", "2023-10-29T21:26:39.024635",
			is_owner, o.getComment()); 
			comments.push_back(comment); 
			//add one to reply_count for parent comment
			for (Comment &c: comments) {
				if (c.getCommentId() == o.getParentCommentId()) {
					c.addReplyCount();
					break; 
				}
			}
		}
		else if (o.getAction() == "like_comment") {//like comment case
			for (Comment &c: comments) {
				if (c.getCommentId() == o.getCommentId()) { 
					c.addLike(); 
					break; 
				}
			}
		}
		else if (o.getAction() == "delete_comment") {//delete comment case
			deleteComment(o.getCommentId()); 
		}
		else if (o.getAction() == "display_comment") {//display comment
			Comment* rootComment = nullptr; 
			for (Comment& c: comments) {
				if (c.getCommentId() == o.getCommentId()) {
					rootComment = &c; 
					break;
				}
			}
			if (rootComment) {
				TreeNode<Comment>* rootNode = new TreeNode<Comment>(*rootComment); 
				rootNode->depth = 0; 
				buildTree(rootNode, *rootComment); 
				printTree(rootNode, outstream); 
				delete rootNode; 
			}
		}
	}
	return 0; 
}