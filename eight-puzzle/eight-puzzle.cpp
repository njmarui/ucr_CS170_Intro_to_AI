#include "priorityqueue.h"
#include <vector>
#include <iostream>
using namespace std;
/*
Write program with
1) Uniform Cost Search1
2) A* with the Misplaced Tile heuristic.
3) A* with the Manhattan Distance heuristic.

//each node need pointer to Parent 
//need formula to create children
//??store child or not??
class Node
    node* Parent
    


function GRAPH-SEARCH(problem) return solution, or failiure to 
    initialize the frontier using the initial state of problem 
    initialize the explored set to empty
    loop do
        if the frontier is empty then return failure
            choose a leaf node and remove it from the frontier
        if the node contains a goal state then return the corresponding 
            solution add the node to the explored set
        expand the chosen node, adding the resulting nodes to the frontier 
        only if not in frontier or explored set
*/


class Node 
{ // binary tree node
    public:
        vector<int> Matrix;
        Node* Parent;
        Node* ThisNode;
        int value;
        int Depth;
        string action;
        Node()
        {
            Matrix.resize(8);
            Parent = NULL;
        }
}; // Node



//~~~~~GLOBALS~~~~~~~~~~~~~~~~~~
int WIDTH = 0;
bool FIRST = true;
int MaxDepth = 0;         
unsigned MaxQueue = 0; 
int NumberNodesExpand = 0;


priorityqueue<Node> WorkingSet;
vector< Node* > Used_Nodes;
//if 9 = 3x3
int SIZE_OF_MATRIX;
vector <int> GoalState;


void print(vector<int> MatrixToPrint)
{
    //Print Function
    for(unsigned i = 0; i < MatrixToPrint.size(); i++)
    {
        if((i != 0) && (i%WIDTH == 0))
        {
            cout << endl;
        }
        cout << MatrixToPrint[i] << ' ';
    }
    cout << endl;
}

void printInOrder(Node* NEXT)
{
    if(NEXT == NULL)
    {
        return;
    }
    
    if(NEXT->Depth > MaxDepth)
    {
        MaxDepth = NEXT->Depth;
    }
    
    
    printInOrder(NEXT->Parent);
    cout << NEXT->action << endl;
    print(NEXT->Matrix);
    cout << endl;
    return;
}

//find and return the index location of the "blank" space
int FindZero(Node NewNode)
{
    for(unsigned i = 0; i < NewNode.Matrix.size(); i++)
    {
        if(NewNode.Matrix[i] == 0)
        {
            return i;
        }
    }
    return 0;
}

//Returns vector with "blank" tile moved left
//if tile is on left edge alrdy return empty vector
vector <int> MoveLeft(Node NewNode, int ZeroLoc)
{
    vector<int> Temp;
    if(ZeroLoc%WIDTH != 0)
    {
        Temp = NewNode.Matrix;
        Temp[ZeroLoc] = NewNode.Matrix[ZeroLoc - 1];
        Temp[ZeroLoc-1] = 0;
    }
    return Temp;
}


//Returns vector with "blank" tile moved right
//if tile is on right edge alrdy return empty vector
vector <int> MoveRight(Node NewNode, int ZeroLoc)
{
    vector<int> Temp;
    if(ZeroLoc%WIDTH != WIDTH-1)
    {
        Temp = NewNode.Matrix;
        Temp[ZeroLoc] = NewNode.Matrix[ZeroLoc + 1];
        Temp[ZeroLoc + 1] = 0;
    }
    return Temp;
}


//Returns vector with "blank" tile moved UP
//if tile is on UP edge alrdy return empty vector
vector <int> MoveUp(Node NewNode, int ZeroLoc)
{
    vector<int> Temp;
    if(ZeroLoc > WIDTH)
    {
        Temp = NewNode.Matrix;
        Temp[ZeroLoc] = NewNode.Matrix[ZeroLoc - WIDTH];
        Temp[ZeroLoc - WIDTH] = 0;
    }
    return Temp;
}


//Returns vector with "blank" tile moved Down
//if tile is on Down edge alrdy return empty vector
vector <int> MoveDown(Node NewNode, int ZeroLoc)
{
    vector<int> Temp;
    if(ZeroLoc + WIDTH < SIZE_OF_MATRIX  )
    {
        Temp = NewNode.Matrix;
        Temp[ZeroLoc] = NewNode.Matrix[ZeroLoc + WIDTH];
        Temp[ZeroLoc + WIDTH] = 0;

    }
    return Temp;
}

//if return true is not used if return false is used
bool CheckUnused(Node NewNode){
    unsigned count = 0;
    //cycle through used nodes
    for(unsigned j = 0; j < Used_Nodes.size(); j++)
    {
        count = 0;
        for(unsigned i = 0; i < NewNode.Matrix.size(); i++)
        {
            if(Used_Nodes[j] == NULL)
            {

            }
            else if(NewNode.Matrix[i] == Used_Nodes[j]->Matrix[i])
            {
                count ++;
            }
            if(count == NewNode.Matrix.size())
            {
                if(NewNode.value < Used_Nodes[j]->value)
                {
                    delete(Used_Nodes[j]);
                    Used_Nodes[j] = NewNode.ThisNode;
                    return true;
                }
                return false;
            }
        }
    }
    return true;
}

//doing UCS implemented in BFS fashion
Node* UniformCostSearch(Node Start)
{

    while(Start.Matrix != GoalState && !WorkingSet.empty())
    {
         NumberNodesExpand ++;
        if(MaxQueue < WorkingSet.M.size())
        {
            MaxQueue = WorkingSet.M.size();
        }
        
        
        Start = WorkingSet.front();
        WorkingSet.pop();
        
        int ZeroLoc = FindZero(Start);
        vector<int> TempVect = MoveUp(Start, ZeroLoc);
        Node *TempNode;
       // cerr << "!!!1";
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.value + 1;
            TempNode->Parent = Start.ThisNode;
            TempNode->ThisNode = TempNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->action = "Up";
            if(CheckUnused(*TempNode))
            {
                
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
               delete(TempNode);
            }
        }
       // cerr << "!!!2";
        TempVect = MoveDown(Start, ZeroLoc);
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.value + 1;
            TempNode->Parent = Start.ThisNode;
            TempNode->ThisNode = TempNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->action = "Down";
            if(CheckUnused(*TempNode))
            {
           //     cerr << "IN!";
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
               delete(TempNode);
            }
        }
       // cerr << "!!!3";
        TempVect = MoveLeft(Start, ZeroLoc);
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.value + 1;
            TempNode->Parent = Start.ThisNode;
            TempNode->ThisNode = TempNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->action = "Left";
            if(CheckUnused(*TempNode))
            {
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
              delete(TempNode);
            }
        }
        //cerr << "!!!4";
        TempVect = MoveRight(Start, ZeroLoc);
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.value + 1;
            TempNode->Parent = Start.ThisNode;
            TempNode->ThisNode = TempNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->action = "Right";
            if(CheckUnused(*TempNode))
            {
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
              delete(TempNode);
            }
        }
        
        if(Start.Depth > 31)
        {
            return NULL;
        }
    }
    if(WorkingSet.empty())
    {
        return NULL;
    }
   // cerr << "!!!5" << endl;
    return Start.ThisNode; 
}


//return number of misplaced tiles
int MisplacedTiles(vector<int> NewVect)
{
    //cerr << WorkingSet.M.size() << ' ';
    //print(NewVect);
    int difference = 0;
    for(unsigned i = 0; i < GoalState.size(); i++)
    {
        if(GoalState[i] != NewVect[i] )
        {
            difference ++;
        }
    }
    
    return difference;
}

// A* search misplaced tile
Node* A_Star_MisTile(Node Start)
{
    
    Start = WorkingSet.front();
    
    while(Start.Matrix != GoalState && !WorkingSet.empty())
    {    
        NumberNodesExpand ++;
        if(MaxQueue < WorkingSet.M.size())
        {
            MaxQueue = WorkingSet.M.size();
        }
        
        
        WorkingSet.pop();
    
        int ZeroLoc = FindZero(Start);
        vector<int> TempVect = MoveUp(Start, ZeroLoc);
        Node *TempNode;
       // cerr << "!!!1";
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.Depth + 1 + MisplacedTiles(TempVect);
            TempNode->Parent = Start.ThisNode;
            TempNode->ThisNode = TempNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->action = "Up";
            if(CheckUnused(*TempNode))
            {
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
                delete(TempNode);
            }
        }
        
        TempVect = MoveDown(Start, ZeroLoc);
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.Depth + 1 + MisplacedTiles(TempVect);
            TempNode->Parent = Start.ThisNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->ThisNode = TempNode;
            TempNode->action = "Down";
            if(CheckUnused(*TempNode))
            {
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
                delete(TempNode);
            }
        }
       // cerr << "!!!3";
        TempVect = MoveLeft(Start, ZeroLoc);
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.Depth + 1 + MisplacedTiles(TempVect);
            TempNode->Parent = Start.ThisNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->ThisNode = TempNode;
            TempNode->action = "Left";
            
            if(CheckUnused(*TempNode))
            {
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
                delete(TempNode);
            }
        }
        //cerr << "!!!4";
        TempVect = MoveRight(Start, ZeroLoc);
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.Depth + 1 + MisplacedTiles(TempVect);
            TempNode->Parent = Start.ThisNode;
            TempNode->ThisNode = TempNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->action = "Right";
            if(CheckUnused(*TempNode))
            {
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
                delete(TempNode);
            }
    
        }
        
        if(Start.Depth > 31)
        {
            return NULL;
        }
        
        Start = WorkingSet.front();
    }
    if(WorkingSet.empty())
    {
        return NULL;
    }
    return Start.ThisNode; 
}


//return number for Manhattan Distance
int Manhattan(vector<int> NewVect)
{
    int ManhattanDis = 0;
    for(unsigned i = 0; i < NewVect.size(); i++)
    {
        int j = i;
        int temp = NewVect[i] - 1 ;
        if(NewVect[i] == 0)
        {
            temp = SIZE_OF_MATRIX - 1;
        }
        ManhattanDis = ManhattanDis + abs((temp%WIDTH) - (j%WIDTH));
        ManhattanDis = ManhattanDis + abs((temp/WIDTH) - (j/WIDTH));
        
    }
    return ManhattanDis;
}

//A* Heuristic search
Node* A_Star_Heuristic(Node Start)
{
    Start = WorkingSet.front();
    while(Start.Matrix != GoalState && !WorkingSet.empty())
    {
        NumberNodesExpand ++;
        if(MaxQueue < WorkingSet.M.size())
        {
            MaxQueue = WorkingSet.M.size();
        }
        
        
        WorkingSet.pop();
        
        int ZeroLoc = FindZero(Start);
        vector<int> TempVect = MoveUp(Start, ZeroLoc);
        Node *TempNode;
       // cerr << "!!!1";
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.Depth + 1 + Manhattan(TempVect);
            TempNode->Parent = Start.ThisNode;
            TempNode->ThisNode = TempNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->action = "Up";
            if(CheckUnused(*TempNode))
            {
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
                delete(TempNode);
            }
        }
        
        TempVect = MoveDown(Start, ZeroLoc);
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.Depth + 1 + Manhattan(TempVect);
            TempNode->Parent = Start.ThisNode;
            TempNode->ThisNode = TempNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->action = "Down";
            if(CheckUnused(*TempNode))
            {
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
                delete(TempNode);
            }
        }
       // cerr << "!!!3";
        TempVect = MoveLeft(Start, ZeroLoc);
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.Depth + 1 + Manhattan(TempVect);
            TempNode->Parent = Start.ThisNode;
            TempNode->ThisNode = TempNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->action = "Left";
            if(CheckUnused(*TempNode))
            {
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
                delete(TempNode);
            }
        }
        //cerr << "!!!4";
        TempVect = MoveRight(Start, ZeroLoc);
        if(!TempVect.empty() )
        {
            //then store this temp vect in a node 
            // push node into priority queue
            TempNode = new Node();
            TempNode->Matrix = TempVect;
            TempNode->value = Start.Depth + 1 + Manhattan(TempVect);
            TempNode->Parent = Start.ThisNode;
            TempNode->ThisNode = TempNode;
            TempNode->Depth = Start.Depth + 1;
            TempNode->action = "Right";
            if(CheckUnused(*TempNode))
            {
                WorkingSet.push(*TempNode, TempNode->value);
                Used_Nodes.push_back(TempNode);
            }
            else
            {
                delete(TempNode);
            }
    
        }
        
        if(Start.Depth > 31)
        {
            return NULL;
        }
        
        Start = WorkingSet.front();
        
    }
    
    if(WorkingSet.empty())
    {
        return NULL;
    }
    return Start.ThisNode; 
}

void GetStartNode()
{
    
    int Y = 0;
    while(Y == 0)
    {
        cout << "Enter width of square matrix (size > 0): "; 
        cin >> Y;
    }
    int count = 0;
    WIDTH = Y;
    Y = Y*Y;
    vector<int> TempVect;
    TempVect.resize(Y,-1);
    while(count < Y)
    {
        int place;
        cout << "What would you like to place " << count << " in the Matrix(0-" << Y-1 << ") " << endl;
        cin >> place;
        if(place > Y)
        {
            cout << "That location is outside the matrix.";
        }
        else if(TempVect[place] != -1)
        {
            cout << "That location is already taken by number " << TempVect[place];
        }
        else
        {
            cout << "Placing " << count << " in Matrix location " << place;
            TempVect[place] = count;
            count ++;
        }
        cout << endl;
    }
    Node* TempNode = new Node();
    TempNode->Matrix = TempVect;
    TempNode->value = 0;
    TempNode->Parent = NULL;
    TempNode->ThisNode = TempNode;
    TempNode->Depth = 0;
    TempNode->action = "Start";
    
    //Use for easy testing Stat Matrix
    //TempNode->Matrix = {0,1,2,5,6,3,4,7,8};
    
    WorkingSet.push(*TempNode, TempNode->value);
    Used_Nodes.push_back(TempNode);
    
    for(int i = 1; i < Y; i++)
    {
		GoalState.push_back(i);
	}
    GoalState.push_back(0);
    SIZE_OF_MATRIX = Y;
}

bool Solvable(Node Start)
{
    int inversion = 0;
    int blankloc = 0;
    for(unsigned i = 0; i < Start.Matrix.size(); i++)
    {
        for(unsigned k = i; k < Start.Matrix.size(); k++)
        {
            if(Start.Matrix[i] == 0)
            {
                blankloc = i;
                k = Start.Matrix.size();
            }
            else if(Start.Matrix[i] > Start.Matrix[k] &&  Start.Matrix[k] != 0)
            {
                
                inversion ++;
            }
        }
    }
    if(WIDTH % 2 == 1 )
    {
        if(inversion % 2  == 0)
        {
            return true;
        }
        
    }
    else
    {
        int temp = SIZE_OF_MATRIX - WIDTH;     
        int blankrow = 1;
        while(blankloc < temp)
        {
            blankrow ++;
            temp = temp - WIDTH;
        }
        
        if(blankrow % 2 == 0 && inversion % 2  == 1)
        {
            return true;
            
        }
        
        if(blankrow % 2 == 1 && inversion % 2  == 0)
        {
            return true;
        }
        
        
        
    }
   
    return false;
}

int main()
{
   cout << "Welcome to 861159446's 8-puzzle solver.\n";
   GetStartNode();
  
   int Mode = 0;
   //choosing which search to use
   while(Mode != 1 && Mode != 2 && Mode != 3)
   {
       Mode = 0;
       cout << "Enter what mode you would like to do the search: \n";
       cout << "1) UniformCostSearch \n2) A* Misplaced Tiles\n3) A* Manhattan\n";
       cin >> Mode;
   }
   
   
   if(!Solvable(WorkingSet.front()))
   {
       Mode = 0;
   }
   
   
   Node* Temp = NULL;
   if(Mode == 1)
   {
        Temp = UniformCostSearch(WorkingSet.front());
   }
   else if(Mode == 2)
   {
        Temp = A_Star_MisTile(WorkingSet.front());
   }
   else if(Mode == 3)
   {
        Temp = A_Star_Heuristic(WorkingSet.front());
   }
   cout << "RESULT \n";
   if(Temp == NULL)
   {
       cout << "There is no path from start state to goat state.\n";
   }
   else
   {
       printInOrder(Temp);
       
       cout <<"To solve this problem the search algorithm expanded a total of "
            << NumberNodesExpand << " nodes." << endl;
        cout << "The maximum number of nodes in the queue at any one time was "
            << MaxQueue << " .\n";
        cout << "The depth of the goal node was " << MaxDepth << ".\n";
   }

}
