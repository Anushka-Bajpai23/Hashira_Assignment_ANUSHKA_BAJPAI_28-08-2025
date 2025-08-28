#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

struct Point {
    int x;
    long long y;
};

class SecretSolver {
private:
    vector<Point> points;
    int n, k;
    
    int charToDigit(char c) {
        if (c >= '0' && c <= '9') return c - '0';
        if (c >= 'a' && c <= 'z') return c - 'a' + 10;
        return 0;
    }
    
    long long convertToDecimal(string value, int base) {
        long long result = 0;
        long long multiplier = 1;
        
        for (int i = value.length() - 1; i >= 0; i--) {
            result += charToDigit(value[i]) * multiplier;
            multiplier *= base;
        }
        return result;
    }
    
    long long findSecret() {
        long long secret = 0;
        
        for (int i = 0; i < k; i++) {
            long long xi = points[i].x;
            long long yi = points[i].y;
            
            double term = yi;
            
            for (int j = 0; j < k; j++) {
                if (i != j) {
                    long long xj = points[j].x;
                    term = term * (0 - xj) / (xi - xj);
                }
            }
            
            secret += (long long)term;
        }
        
        return secret;
    }
    
public:
    bool readFromFile(string filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "Error: Could not open file " << filename << endl;
            return false;
        }
        
        string json((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();
        
        parseInput(json);
        return true;
    }
    
    void parseInput(string json) {
        cout << "\n=== PROBLEM: Find Secret using Shamir's Secret Sharing ===" << endl;
        cout << "Given polynomial roots in JSON format with encoded y-values" << endl;
        cout << "Need to decode y-values and find constant term 'c' of polynomial" << endl;
        
        cout << "\nInput JSON:" << endl;
        cout << json << endl;
        
        size_t nPos = json.find("\"n\":");
        size_t kPos = json.find("\"k\":");
        
        if (nPos != string::npos) {
            size_t start = json.find(':', nPos) + 1;
            size_t end = json.find_first_of(",}", start);
            string nStr = json.substr(start, end - start);
            n = stoi(nStr);
        }
        
        if (kPos != string::npos) {
            size_t start = json.find(':', kPos) + 1;
            size_t end = json.find_first_of(",}", start);
            string kStr = json.substr(start, end - start);
            k = stoi(kStr);
        }
        
        cout << "\nn = " << n << " (total points given)" << endl;
        cout << "k = " << k << " (minimum points needed)" << endl;
        cout << "Polynomial degree = " << k-1 << endl;
        
        cout << "\n=== STEP 1: Decode Y Values ===" << endl;
        
        for (int i = 1; i <= n; i++) {
            string key = "\"" + to_string(i) + "\"";
            size_t keyPos = json.find(key);
            
            if (keyPos != string::npos) {
                size_t baseStart = json.find("\"base\":", keyPos);
                baseStart = json.find('\"', baseStart + 7) + 1;
                size_t baseEnd = json.find('\"', baseStart);
                string baseStr = json.substr(baseStart, baseEnd - baseStart);
                int base = stoi(baseStr);
                
                size_t valueStart = json.find("\"value\":", keyPos);
                valueStart = json.find('\"', valueStart + 8) + 1;
                size_t valueEnd = json.find('\"', valueStart);
                string value = json.substr(valueStart, valueEnd - valueStart);
                
                Point p;
                p.x = i;
                p.y = convertToDecimal(value, base);
                points.push_back(p);
                
                cout << "x = " << i << ", encoded = '" << value << "' (base " << base << ") -> decoded y = " << p.y << endl;
            }
        }
    }
    
    void solve() {
        cout << "\n=== STEP 2: Apply Lagrange Interpolation ===" << endl;
        cout << "Lagrange Interpolation Formula:" << endl;
        cout << "f(x) = sum of [yi * product of (x-xj)/(xi-xj)] for all i, j where i?j" << endl;
        cout << "To find secret: f(0) = sum of [yi * product of (-xj)/(xi-xj)]" << endl;
        
        cout << "\nPoints used for calculation:" << endl;
        for (int i = 0; i < k; i++) {
            cout << "(" << points[i].x << ", " << points[i].y << ")" << endl;
        }
        
        long long secret = findSecret();
        
        cout << "\n=== STEP 3: Calculate Secret ===" << endl;
        cout << "Using first " << k << " points to reconstruct polynomial" << endl;
        cout << "The polynomial is: f(x) = c + bx + ax^2 + ..." << endl;
        cout << "Secret = f(0) = c (constant term)" << endl;
        
        cout << "\n=== FINAL ANSWER ===" << endl;
        cout << "c = " << secret << endl;
    }
};

int main() {
    SecretSolver solver;
    
    cout << "=== ASSIGNMENT: Read Test Case from JSON File ===" << endl;
    
    cout << "\n--- TEST CASE 1 ---" << endl;
    if (solver.readFromFile("testcase1.json")) {
        solver.solve();
    }
    
    cout << "\n\n--- TEST CASE 2 ---" << endl;
    SecretSolver solver2;
    if (solver2.readFromFile("testcase2.json")) {
        solver2.solve();
    }
    
    return 0;
}