#include <string>
#include <algorithm>
#include <vector>
#include <iostream>

class Comment {
public: 
	//constructor (takes jsonFile as input)
	Comment(std::string json_content) {
		//assign values to variables
		size_t pos = 0; 
		size_t keyEndPos; 
		while (pos < json_content.length()) {
			
			//find the key
			size_t keyStartPos = json_content.find("\"", pos); 
			if (keyStartPos == std::string::npos) {break;}
			keyStartPos++; 
			keyEndPos = json_content.find("\"", keyStartPos); 
			if (keyEndPos == std::string::npos) {break;}
			std::string key = json_content.substr(keyStartPos, keyEndPos - keyStartPos);
			
			//find the value
			bool no_string = false; 
			size_t valueStartPos = json_content.find("\"", keyEndPos+1); 
			if (valueStartPos == std::string::npos) {break;}
			if (json_content.find(",", keyEndPos+1) < valueStartPos) {
				valueStartPos = keyEndPos + 2;
				no_string = true; 
			}
			valueStartPos++; 
			size_t valueEndPos = json_content.find("\"", valueStartPos); 
			if (no_string) {
				valueEndPos = json_content.find(",", valueStartPos);
			}
			if (valueEndPos == std::string::npos) {break;}
			std::string value = json_content.substr(valueStartPos, valueEndPos - valueStartPos);
			//empty string case
			if (value == "") {value = "\"\"";}
			else {
				if (value.find('"') != std::string::npos) {
					value.erase(std::remove(value.begin(), value.end(), '"'), value.end());
				}
			}
			
			//set variables
			if (key == "video_id")
                video_id = value;
            else if (key == "author")
                author = value;
            else if (key == "comment_id")
                comment_id = value;
            else if (key == "like_count")
                like_count = std::stoi(value);
            else if (key == "reply_count")
                reply_count = std::stoi(value);
            else if (key == "is_reply")
                is_reply = (value == "true");
            else if (key == "parent_comment_id")
                parent_comment_id = value;
            else if (key == "published_date")
                published_date = value;
            else if (key == "crawled_date")
                crawled_date = value;
            else if (key == "is_video_owner")
                is_video_owner = (value == "true");
            else if (key == "comment")
                commentStr = value;
			pos = valueEndPos + 1; 
		}
		
	}
	
	Comment(std::string video_id, std::string author, std::string comment_id, int like_count, int reply_count, 
	bool is_reply, std::string parent_comment_id, std::string published_date, std::string crawled_date, 
	bool is_video_owner, std::string commentStr) {
		this->video_id = video_id;
		this->author = author;
		this->comment_id = comment_id; 
		this->like_count = like_count; 
		this->reply_count = reply_count; 
		this->is_reply = is_reply; 
		this->parent_comment_id = parent_comment_id; 
		this->published_date = published_date; 
		this->crawled_date = crawled_date; 
		this->is_video_owner = is_video_owner; 
		this->commentStr = commentStr; 
	}
	
	//accessors
	 std::string getVideoId() const {
        return video_id;
    }

    std::string getAuthor() const {
        return author;
    }

    std::string getCommentId() const {
        return comment_id;
    }

    int getLikeCount() const {
        return like_count;
    }
	
	void addLike() {
		like_count++;
	}

    int getReplyCount() const {
        return reply_count;
    }
	
	void addReplyCount() {
		reply_count++; 
	}
	
	void setReplyCount(int num) {
		reply_count = num; 
	}

    bool getIsReply() const {
        return is_reply;
    }

    std::string getParentCommentId() const {
        return parent_comment_id;
    }

    std::string getPublishedDate() const {
        return published_date;
    }
	
	void setPublishedDate(std::string date){
		published_date = date;
	}

    std::string getCrawledDate() const {
        return crawled_date;
    }

    bool getIsVideoOwner() const {
        return is_video_owner;
    }

    std::string getComment() const {
        return commentStr;
    }
	
private:
	std::string video_id, author, comment_id, parent_comment_id, published_date, crawled_date, commentStr; 
	int like_count, reply_count; 
	bool is_reply, is_video_owner;
};

