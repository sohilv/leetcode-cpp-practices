//backtracking
//TLE
//3 / 20 test cases passed.
class Solution {
public:
    vector<char> operators;
    
    bool stringIsNumber(string& s){
        return !s.empty() && s.find_first_not_of("0123456789") == std::string::npos;
    }
    
    long long evaluate(string& expr){
        // cout << "expr: " << expr << endl;
        deque<string> deq;
        //'*' has highest priority, process it immediately after we meet it
        for(int i = 0; i < expr.size(); i++){
            char c = expr[i];
            if(c == '*'){
                long long operand1 = stoll(deq.back()); deq.pop_back();
                long long operand2 = expr[++i] - '0';
                // cout << "meet * " << operand1 << " and " << operand2 << endl;
                //calculate immediately and push to deq
                deq.push_back(to_string(operand1 * operand2));
            }else if(!deq.empty() && stringIsNumber(deq.back()) && isdigit(c)){
                //extend previous number
                deq.back() += c;
            }else{
                deq.push_back(string(1, c));
            }
            // cout << "deq: ";
            // for(string s : deq){
            //     cout << s << " ";
            // }
            // cout << endl;
        }
        
        //no '+' or '-' in expr
        if(deq.size() == 1){
            long long res = stoll(deq.back());
            // cout << "res: " << res << endl;
            return res;
        }
        
        //now there are only '+' and '-'
        
        /*
        important!
        now access the deque from front!
        because when there are + and -, 
        expressions should be evaluated in FIFO order
        */
        long long res = stoll(deq.front()); deq.pop_front();
        while(!deq.empty() && stringIsNumber(deq.front())){
            res = res * 10 + stoll(deq.front()); deq.pop_front();
        }
        
        while(!deq.empty()){
            //pop one operator and one operand at a time
            char c = deq.front()[0]; deq.pop_front();
            long long operand2 = stoll(deq.front()); deq.pop_front();
            
            if(c == '+'){
                res = res + operand2;
            }else if(c == '-'){
                res = res - operand2;
            }
        }
        
        // cout << "res: " << res << endl;
        
        return res;
    }
    
    void backtrack(vector<string>& exprs, string& expr, string& num, int target, int start){
        if(start == num.size()){
            if(evaluate(expr) == target){
                exprs.push_back(expr);
            }
        }else{
            for(char op : operators){
                if(!expr.empty() && expr.back() == '0' && op == '\0'){
                    //avoid generating "1+05"
                    continue;
                }
                if(op != '\0') expr += op;
                expr += num[start];
                backtrack(exprs, expr, num, target, start+1);
                // cout << "before erase: " << expr << endl;
                //erase two chars if we have added op
                expr.erase(expr.size()-(1 + (op != '\0')), 1 + (op != '\0'));
                // cout << "after erase: " << expr << endl;
            }
        }
    }
    
    vector<string> addOperators(string num, int target) {
        //'\0' means add nothing
        operators = {'\0', '+', '-', '*'};
        vector<string> exprs;
        if(num.size() == 0) return {""};
        string expr(1, num[0]);
        
        backtrack(exprs, expr, num, target, 1);
        
        return exprs;
    }
};

//backtrack, evaluate expression on the fly
//WA
//14 / 20 test cases passed.
class Solution {
public:
    vector<char> operators;
    
    void backtrack(vector<string>& exprs, string& expr, string& num, int target, int start, long long res, int lastOperand, char lastOperator){
        // cout << "expr: " << expr << ", start: " << start << ", res: " << res << ", lastOperand: " << lastOperand << ", lastOp: " << lastOperator << endl;
        //res: current evaluated result of expr
        if(start == num.size()){
            // cout << "expr: " << expr << endl;
            //expr is already evaluated on the fly!
            if(res == target){
                exprs.push_back(expr);
            }
        // }else if(log10(res) - (num.size() - start) > log10(target)+1){
        //     //early stopping
        //     cout << res << ", " << start << ", " << target << endl;
        }else{
            int digit = num[start] - '0';
            for(char op : operators){
                int lastRes = res;
                // cout << "curOp: " << op << ", res: " << res << ", lastOp: " << lastOperator << ", lastOperand: " << lastOperand << endl;
                if(!expr.empty() && expr.back() == '0' && op == '\0'){
                    //avoid generating "1+05"
                    continue;
                }
                if(op != '\0') expr += op;
                expr += num[start];
                //calculate res on the fly!
                switch(op){
                    case '+':
                        res += digit;
                        // lastOperator = '+';
                        break;
                    case '-':
                        res -= digit;
                        // lastOperator = '-';
                        break;
                    case '*':
                        //special case, '*' has higher precedence!
                        switch(lastOperator){
                            case '+':
                                //undo previous operation
                            	res -= lastOperand;
                                //'*' has higher precedence than '+'
                                res = res + lastOperand * digit;
                                break;
                            case '-':
                                //undo previous operation
                            	res += lastOperand;
                                //'*' has higher precedence than '-'
                                res = res - lastOperand * digit;
                                break;
                            case '*':
                                //what if "10+2*3*4"?
                            case '\0':
                                //directly multiply?
                                res *= digit;
                        }
                        /*
                        don't change lastOperator, only change lastOperand
                        since there is no chance that we undo the '*' operation!
                        */
                        // lastOperand *= digit;
                        // curOperand = lastOperand * digit;
                        //lastOperator = '*';
                        break;
                    case '\0':
                        //special case, '*' has higher precedence!
                        switch(lastOperator){
                            case '+':
                                //undo previous operation
                            	res -= lastOperand;
                                //'\0' has higher precedence than '+'
                                res = res + (lastOperand * 10 + digit);
                                break;
                            case '-':
                                //undo previous operation
                            	res += lastOperand;
                                //'\0' has higher precedence than '-'
                                res = res - (lastOperand * 10 + digit);
                                break;
                            case '*':
                            case '\0':
                                //directly update?
                                res = (res * 10 + digit);
                        }
                        // lastOperator = '\0';
                        break;
                }
                // lastOperand = digit;
                backtrack(exprs, expr, num, target, start+1, res, 
                          (lastOperator == '*' && op == '*') ? lastOperand * digit: digit, 
                          ((op == '*') ? lastOperator: op));
                // cout << "before erase: " << expr << endl;
                //erase two chars if we have added op
                expr.erase(expr.size()-(1 + (op != '\0')), 1 + (op != '\0'));
                res = lastRes;
                // cout << "after erase: " << expr << endl;
            }
        }
    }
    
    vector<string> addOperators(string num, int target) {
        //'\0' means add nothing
        operators = {'\0', '+', '-', '*'};
        vector<string> exprs;
        if(num.size() == 0) return {""};
        string expr(1, num[0]);
        int digit = num[0] - '0';
        
        backtrack(exprs, expr, num, target, 1, digit, digit, '\0');
        
        return exprs;
    }
};
