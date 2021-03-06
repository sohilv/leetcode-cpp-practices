//DP
//Runtime: 72 ms, faster than 51.00% of C++ online submissions for Number of Longest Increasing Subsequence.
//Memory Usage: 13 MB, less than 11.11% of C++ online submissions for Number of Longest Increasing Subsequence.
//time: O(N^2), space: O(N)
class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        int n = nums.size();
        if(n <= 1) return n;
        vector<int> lengths(n, 0);
        vector<int> counts(n, 1);
        
        int maxlen = 0;
        int ans = 0;
        
        for(int j = 0; j < n; j++){
            for(int i = 0; i < j; i++){
                //[0...i] and [0...j] which j > i
                if(nums[i] < nums[j]){
                    if(lengths[i] >= lengths[j]){
                        /*
                        lengths[i] + 1 > lengths[j]
                        so we discard the previously found LIS ending at nums[j],
                        instead just use what extends from LIS ending at nums[i]
                        */
                        lengths[j] = lengths[i] + 1;
                        //LIS ending at j is extending from LIS ending at i
                        counts[j] = counts[i];
                    }else if(lengths[i] + 1 == lengths[j]){
                        /*
                        [0...j] can be generated by [0...i] plus nums[j]
                        the previous found LIS ending at j are reserved
                        */
                        counts[j] += counts[i];
                    }
                }
            }
            
            if(maxlen < lengths[j]){
                /*
                current found LIS's length is the longest,
                so overwrite the info about what we previous found
                */
                maxlen = lengths[j];
                ans = counts[j];
            }else if(maxlen == lengths[j]){
                //accumulate the count of LIS with length equal to maxlen
                ans += counts[j];
            }
        }
        
        return ans;
    }
};

//
class NodeValue{
public:
    int length;
    int count;
    NodeValue(int l, int c) : length(l), count(c) {}
};

class Node{
public:
    int range_left, range_right;
    Node *left, *right;
    NodeValue* val;
    
    Node(int rl, int rr){
        range_left = rl;
        range_right = rr;
        left = nullptr;
        right = nullptr;
        //length 0, count 1
        val = new NodeValue(0, 1);
    }
    
    int getRangeMid(){
        return range_left + (range_right - range_left)/2;
    }
    
    Node* getLeft(){
        if(left == nullptr) left = new Node(range_left, getRangeMid());
        return left;
    }
    
    Node* getRight(){
        if(right == nullptr) right = new Node(getRangeMid()+1, range_right);
        return right;
    }
};

//Approach 2: Segment Tree
//https://leetcode.com/articles/number-of-longest-increasing-subsequence/
//Runtime: 72 ms, faster than 52.97% of C++ online submissions for Number of Longest Increasing Subsequence.
//Memory Usage: 60.9 MB, less than 11.11% of C++ online submissions for Number of Longest Increasing Subsequence.
//time: O(NlogN), space: O(N)
//segment tree
class Tree{
private:
    NodeValue* merge(NodeValue* v1, NodeValue* v2){
        if(v1->length == v2->length){
            //we cannot accumulate the count of length 0 sequence
            if(v1->length == 0) return new NodeValue(0, 1);
            return new NodeValue(v1->length, v1->count + v2->count);
        }
        return (v1->length > v2->length) ? v1 : v2;
    }
    
    void insert(Node* node, int key, NodeValue* value){
        if(node->range_left == node->range_right){
            //leaf node
            node->val = merge(node->val, value);
            return;
        }
        
        if(key <= node->getRangeMid()){
            //the queried key falls in left subtree
            insert(node->getLeft(), key, value);
        }else{
            //the queried key falls in right subtree
            insert(node->getRight(), key, value);
        }
        
        node->val = merge(node->getLeft()->val, node->getRight()->val);
    }
    
    NodeValue* query(Node* node, int key){
        if(key >= node->range_right){
            /*
            we are querying [minv, key],
            and current node's range is completely included in the query range
            */
            return node->val;
        }else if(key < node->range_left){
            //query range is outside the node's range
            return new NodeValue(0, 1);
        }
        
        return merge(query(node->getLeft(), key), query(node->getRight(), key));
    }

public:
    Node* root;
    
    Tree(int rl, int rr){
        root = new Node(rl, rr);
    };
    
    //used outside
    void insert(int key, NodeValue* value){
        insert(root, key, value);
    };
    
    NodeValue* query(int key){
        return query(root, key);
    };
};

class Solution {
public:
    int findNumberOfLIS(vector<int>& nums) {
        if(nums.size() == 0) return 0;
        int minv = INT_MAX, maxv = INT_MIN;
        
        for(int num : nums){
            minv = min(minv, num);
            maxv = max(maxv, num);
        }
        
        Tree* tree = new Tree(minv, maxv);
        for(int num : nums){
            NodeValue* v = tree->query(num-1);
            tree->insert(num, new NodeValue(v->length+1, v->count));
        }
        
        return tree->root->val->count;
    }
};
