/**
 * @file sweep_line.cpp
 * @brief Line Segment Intersections
 * @date 2022-03-22
 **************************************/


#include <iostream> 
#include<bits/stdc++.h>

#define lft -1
#define rt 1
#define RED 2
#define BLACK 3

using namespace std;


/**
 * @brief Structure of a 2-D point 
 **************************************/
struct point{
	double x,y;  /// coordinates of point
	int segId=-1; /// Id of segment-1 on which point lies
	int segId1=-1; /// Id of segment-2 on which point lies
	int loc=0; /// indicates location of point on segment
	point(){
		x=0;
		y=0;
		segId=-1;
		loc=0;
	}
	point(double x1,double y1){
		x=x1;
		y=y1;
	}
	point(double x1,double y1,int sid,int l){
		x=x1;
		y=y1;
		segId = sid;
		loc=l;
	}
	point(double x1,double y1,int sid,int sid1,int l){
		x=x1;
		y=y1;
		segId = sid;
		segId1=sid1;
		loc=l;
	}
};


/**
 * @brief Structure of a line segment 
 **************************************/
struct Segment{
	point p,q; ///p = left , q = right
	int segId; 
	Segment(){
        segId=-1;
	}
	Segment(point p1,point q1,int sid){
		p = p1;
        q = q1;
        segId = sid;
	}
	Segment(double x_l,double y_l,double x_r,double y_r,int sid){
		p = point(x_l,y_l);
        q = point(x_r,y_r);
        segId = sid;
	}
	

};


/**
 * @brief Structure of node in eventQueue 
 **************************************/
struct eventNode{
	point data;
	int color; 
	eventNode* par;
    eventNode* left;
	eventNode* right;
    eventNode(point p){
    	this->data=p;
    	par=NULL;
    	left=NULL;
    	right=NULL;
    	this->color=RED; 
    }
};



/**
 * @brief eventQueue to maintain the ordering of points according to x-coordinates
 * @details Implements a red-black binary search tree (balanced BST) to maintain the ordered points. Supports insert and search operations. 
 **************************************/
class eventQueue{
	eventNode*p;
public:
	eventQueue(){
		p=NULL;
	}


    /**
     * @brief function to get the root of BBST
     **************************************/
	eventNode*getRoot(){
		return p;
	}
    
    /**
     * @brief function to insert node in eventQueue
     **************************************/
	eventNode* insert(eventNode*p,eventNode*q){
		if(p==NULL){
			return q;
		}	
		if(((q->data).x)<((p->data).x)){
			p->left = insert(p->left,q);
			p->left->par=p;
		}
		else if(((q->data).x)>((p->data).x)){
			p->right = insert(p->right,q);
			p->right->par=p;
		}
		else{
			if(((q->data).y)<((p->data).y)){
				p->left = insert(p->left,q);
			    p->left->par=p;
			}
			else{
				p->right = insert(p->right,q);
			    p->right->par=p;
			}
		}

		return p;
	}

	/**
     * @brief function left rotate about a node in BBST
     **************************************/
    void leftRotate(eventNode*&p,eventNode*&q){
    	eventNode * tmp = q->right;
    	q->right = tmp->left;
    	if(q->right){q->right->par=q;}
    	tmp->par = q->par;
        if(!q->par){p=tmp;}
        else if(q->par->right==q){
        	q->par->right=tmp;
        }
        else if(q->par->left==q){
        	q->par->left=tmp;
        }
    	tmp->left=q;
    	tmp->left->par = tmp;
    }


    /**
     * @brief function right rotate about a node in BBST
     **************************************/
    void rightRotate(eventNode*&p,eventNode*&q){
    	eventNode * tmp = q->left;
    	q->left = tmp->right;
    	if(q->left){q->left->par=q;}
    	tmp->par = q->par;
        if(!q->par){p=tmp;}
        else if(q->par->right==q){
        	q->par->right=tmp;
        }
        else if(q->par->left==q){
        	q->par->left=tmp;
        }
    	tmp->right=q;
    	tmp->right->par = tmp;

    }

    /**
     * @brief function fix the anamolies/violations in BBST arising due to insert operation
     **************************************/
    void fixRedParentAnamoly(eventNode*&p,eventNode*&q){

    	eventNode* parent = NULL,*grandparent=NULL;
    	/// Violation would occur if parent node is red 
    	while((q!=p)&&(q->color==RED)&&(q->par->color==RED)){
    		parent = q->par;
    		grandparent = q->par->par;
    		///Case 1 : parent is left child of grandparent
    		if(parent==grandparent->left){
    			eventNode* uncle = grandparent->right; /// sibling of parent
    			 ///Case 1.1 : uncle node is red : push the violation upward 
    			 if(uncle&&uncle->color==RED){
    			 	parent->color=BLACK;
    			 	uncle->color=BLACK;
    			 	grandparent->color=RED;
    			 	q = grandparent;
    			 }
    			 /// Case 1.2 : uncle node is black
    			 else{
    			 	/// Case 1.2.1
    			 	if(q==parent->right){
    			 		leftRotate(p,parent);
    			 		q=parent;
    			 		parent=q->par;
    			 	}
    			 	/// Case 1.2.2
                    swap(parent->color,grandparent->color);
                    rightRotate(p,grandparent);
                    q = parent;

    			 }


    		}
    		///Case 2 : parent is right child of grandparent 
    		else
    		{
    			eventNode* uncle= grandparent->left;
    			/// Case 2.1
    			if(uncle&&uncle->color==RED){
    			 	parent->color=BLACK;
    			 	uncle->color=BLACK;
    			 	grandparent->color=RED;
    			 	q = grandparent;
    			 }
    			 /// Case 2.2
    			 else{
    			 	/// Case 2.2.1
    			 	if(q==parent->left){
    			 		rightRotate(p,parent);
    			 		q=parent;
    			 		parent=q->par;
    			 	}
    			 	/// Case 2.2.2 
                    swap(parent->color,grandparent->color);
                    leftRotate(p,grandparent);
                    q = parent;


    			 }



    		}

    	}

        p->color=BLACK;
    }


    /**
     * @brief entry-function facilitate insertion of a point in eventQueue
     **************************************/
	void balancedInsert(point & data){
		eventNode*q = new eventNode(data);
		p = insert(p,q);
		fixRedParentAnamoly(p,q);
	}

    /**
     * @brief function to indicate presence a node in eventQueue
     * @return returns boolean value
     **************************************/
    bool bbst_search(eventNode*p,point data){
    	if(p==NULL){
    		return false;
    	}
    	if(p->data.segId==data.segId){
    		return true;
    	}
    	return bbst_search(p->left,data)||bbst_search(p->right,data);
    }

    /**
     * @brief entry-function facilitate searching of a point in eventQueue
     **************************************/
	bool search(point data){
		bool ans = bbst_search(p,data);
		return ans;
	}


    /**
     * @brief performs inorder traversal of binary tree to get the points in order
     **************************************/
    void traverse(eventNode*p){
    	if(p==NULL){
    		return;
    	}
    	traverse(p->left);
    	cout<<"("<<p->data.x<<","<<p->data.y<<")"<<endl;
    	traverse(p->right);

    }

    /**
     * @brief entry-point to perform inorder traversal
     **************************************/
    void inorder(){
    	traverse(p);
    }


};


/**
 * @brief structure of node in sweepLineStatus data structure
 **************************************/
struct statusNode{
	Segment data;
	int color;
	statusNode* par;
    statusNode* left;
	statusNode* right;
    statusNode(Segment seg){
    	this->data=seg;
    	par=NULL;
    	left=NULL;
    	right=NULL;
    	this->color=RED; 
    }
};

/**
 * @brief sweepLinesStatus structure to maintain the ordering of line segments according to order intersected by sweep-line
 * @details Implements a red-black binary search tree (balanced BST) to maintain the ordered segments. Supports insert , search and delete operations. 
 **************************************/
class sweepLineStatus{
	statusNode*p;
public:
	sweepLineStatus(){
		p=NULL;
	}

	/**
     * @brief function to get root node of sweepLineStatus BBST
     **************************************/
	statusNode*getRoot(){
		return p;
	}

	/**
     * @brief function to check if a line is below another line along the sweep line
     **************************************/
	bool isBelow(point a,Segment s){
		double a1 = s.p.y - s.q.y;
		double b1 = s.q.x - s.p.x;
		double c1 = s.p.x*s.q.y - s.q.x*s.p.y;
		double x1 = a.x;
		double y1 = a.y;
		return (a1*x1+b1*y1+c1<0 and b1>0) or (a1*x1+b1*y1+c1>0 and b1<0);
	}

	/**
     * @brief function to check if a line is above another line along the sweep line
     **************************************/
	bool isAbove(point a,Segment s){
		double a1 = s.p.y - s.q.y;
		double b1 = s.q.x - s.p.x;
		double c1 = s.p.x*s.q.y - s.q.x*s.p.y;
		double x1 = a.x;
		double y1 = a.y;
		return (a1*x1+b1*y1+c1>0 and b1>0) or (a1*x1+b1*y1+c1<0 and b1<0);
	}
    
    /**
     * @brief function to insert line segment into sweepLineStatus structure
     **************************************/
	statusNode* insert(statusNode*p,statusNode*q){
		if(p==NULL){
			return q;
		}	
		if(isBelow(q->data.p,p->data)){
			p->left = insert(p->left,q);
			p->left->par=p;
		}
		else if(isAbove(q->data.p,p->data)){
			p->right = insert(p->right,q);
			p->right->par=p;
		}
		return p;
	}
    
    /**
     * @brief function perform left rotation about a node in RBT
     **************************************/
    void leftRotate(statusNode*&p,statusNode*&q){
    	statusNode * tmp = q->right;
    	q->right = tmp->left;
    	if(q->right){q->right->par=q;}
    	tmp->par = q->par;
        if(!q->par){p=tmp;}
        else if(q->par->right==q){
        	q->par->right=tmp;
        }
        else if(q->par->left==q){
        	q->par->left=tmp;
        }
    	tmp->left=q;
    	tmp->left->par = tmp;
    }

    /**
     * @brief function perform right rotation about a node in RBT
     **************************************/
    void rightRotate(statusNode*&p,statusNode*&q){
    	statusNode * tmp = q->left;
    	q->left = tmp->right;
    	if(q->left){q->left->par=q;}
    	tmp->par = q->par;
        if(!q->par){p=tmp;}
        else if(q->par->right==q){
        	q->par->right=tmp;
        }
        else if(q->par->left==q){
        	q->par->left=tmp;
        }
    	tmp->right=q;
    	tmp->right->par = tmp;

    }

    /**
     * @brief function to fix anamolies and violations due to insert operartions in RBT
     **************************************/
    void fixRedParentAnamoly(statusNode*&p,statusNode*&q){

    	statusNode* parent = NULL,*grandparent=NULL;
    	/// Violation would occur if parent node is red 
    	while((q!=p)&&(q->color==RED)&&(q->par->color==RED)){
    		parent = q->par;
    		grandparent = q->par->par;
    		/// Case 1 : parent is left child of grandparent
    		if(parent==grandparent->left){
    			statusNode* uncle = grandparent->right; /// sibling of parent
    			 /// Case 1.1 : uncle node is red : push the violation upward
    			 if(uncle&&uncle->color==RED){
    			 	parent->color=BLACK;
    			 	uncle->color=BLACK;
    			 	grandparent->color=RED;
    			 	q = grandparent;
    			 }
    			 /// Case 1.2 : uncle node is black
    			 else{
    			 	/// Case 1.2.1 
    			 	if(q==parent->right){
    			 		leftRotate(p,parent);
    			 		q=parent;
    			 		parent=q->par;
    			 	}
    			 	/// Case 1.2.2
                    swap(parent->color,grandparent->color);
                    rightRotate(p,grandparent);
                    q = parent;

    			 }


    		}
    		/// Case 2 : parent is right child of grandparent 
    		else
    		{
    			statusNode* uncle= grandparent->left;
    			/// Case 2.1
    			if(uncle&&uncle->color==RED){
    			 	parent->color=BLACK;
    			 	uncle->color=BLACK;
    			 	grandparent->color=RED;
    			 	q = grandparent;
    			 }
    			 /// Case 2.2
    			 else{
    			 	/// Case 2.2.1 
    			 	if(q==parent->left){
    			 		rightRotate(p,parent);
    			 		q=parent;
    			 		parent=q->par;
    			 	}
    			 	/// Case 2.2.2
                    swap(parent->color,grandparent->color);
                    leftRotate(p,grandparent);
                    q = parent;


    			 }



    		}

    	}

        p->color=BLACK;
    }
    
    /**
     * @brief entry-function to perform insertion
     **************************************/
	void balancedInsert(Segment & data){
		statusNode*q = new statusNode(data);
		p = insert(p,q);
		fixRedParentAnamoly(p,q);
	}

	/**
     * @brief function to indicate presence a node in sweepLineStatus
     * @return returns boolean value
     **************************************/
	bool bbst_search(statusNode*p,Segment data){
    	if(p==NULL){
    		return false;
    	}
    	if(p->data.segId==data.segId){
    		return true;
    	}
    	return bbst_search(p->left,data)||bbst_search(p->right,data);
    }

    /**
     * @brief entry-function to indicate presence a node
     **************************************/
	bool search(Segment data){
		bool ans = bbst_search(p,data);
		return ans;
	}

	/**
     * @brief function to search for a node in sweepLineStatus structure
     * @return returns pointer to the node having the segment
     **************************************/
	statusNode*get_node(statusNode*p,Segment data){
		if(p==NULL){
    		return NULL;
    	}
    	if(p->data.segId==data.segId){
    		return p;
    	}
    	statusNode*l = get_node(p->left,data);
    	if(l!=NULL){
    		return l;
    	}
    	statusNode*r = get_node(p->right,data);
    	if(r!=NULL){
    		return r;
    	}

    	return NULL;
	}

	/**
     * @brief function to get line segment above another line segment in sweepLineStatus structure
     * @return returns segment id of above segment
     **************************************/
    int get_above_segment(Segment data){
		statusNode*req = p;
		int ans=-1;
		while(req!=NULL){
			if(isAbove(data.p,p->data)){
				req=req->right;
			}
			else{
				if(req->data.segId!=data.segId){
				ans = req->data.segId;}
				req=req->left;
			}
		}
	    return ans;
	}

	/**
     * @brief function to get line segment below another line segment in sweepLineStatus structure
     * @return returns segment id of below segment
     **************************************/
	int get_below_segment(Segment data){
		statusNode*req = p;
		int ans=-1;
		while(req!=NULL){
			if(isBelow(data.p,p->data)){
				req=req->left;
			}
			else{
				if(req->data.segId!=data.segId){
				ans = req->data.segId;}
				req=req->right;
			}
		}
	    return ans;
	}

	statusNode*successor(statusNode*target){
		statusNode*x=target;
		while(x->left){
			x=x->left;
		}
		return x;
	}

    /**
     * @brief entry-function to delete a node
     **************************************/
	void delete_node(Segment data){
		if(p==NULL){
			return;
		}
		statusNode* target = get_node(p,data);
		balancedRemove(p,target);
	}
    
    /**
     * @brief performs delete operation of the node in RBT
     **************************************/
	void balancedRemove(statusNode*&p,statusNode*&target){
		statusNode*u=NULL;
		if(target->left&&target->right){
			u = successor(target);
		}
		else if(target->left){
           u = target->left;
		}
		else if(target->right){
			u = target->right;
		}
		statusNode*parent = target->par;
		int doubleBlack=0;
		if(((u==NULL)||(u->color==BLACK))&&(target->color==BLACK)){
           doubleBlack=1;
		}
		if(!u){
			if(target==p){
				p=NULL;
			}
			else{
				if(doubleBlack){
					fixDBAnamoly(p,target);
				}
				else{
					statusNode*sibling=NULL;
					if(target->par->left==target){
						sibling=target->par->right;
					} 
					else if(target->par->right==target){
						sibling=target->par->left;
					}
					if(sibling){
						sibling->color=RED;
					}
				}
				if(target->par->left==target){
					parent->left=NULL;
				}
				else{
					parent->right=NULL;
				}
			}
			delete target;
			return;
		}

		if(target->left==NULL||target->right==NULL){
			if(target==p){
				target->data = u->data;
				target->left=NULL;
				target->right=NULL;
				delete u;
			}
			else{
				if(target->par->left==target){
					parent->left=u;
				}
				else{
					parent->right=u;
				}
				delete target;
				u->par = parent;
				if(doubleBlack){
					fixDBAnamoly(p,u);
				}
				else{
					u->color=BLACK;
				}
			}
			return;
		}
		Segment tmpdata;
		tmpdata = u->data;
		u->data = target->data;
		target->data=tmpdata;
		balancedRemove(p,u);
	}
    
    /**
     * @brief function to fix the anamolies after delete operation
     **************************************/
	void fixDBAnamoly(statusNode*&p,statusNode*&target){
		if(target==p){
			return;
		}
        statusNode*sibling=NULL;
		if(target->par->left==target){
			sibling=target->par->right;
		} 
		else if(target->par->right==target){
			sibling=target->par->left;
		}
		statusNode*parent = target->par;
		if(!sibling){
			fixDBAnamoly(p,parent);
		}
		else{
			if(sibling->color==RED){
				parent->color=RED;
				sibling->color=BLACK;
				if(sibling->par->left==sibling){
					rightRotate(p,parent);
				}
				else{
					leftRotate(p,parent);
				}
				fixDBAnamoly(p,target);
			}
			else{
				if((sibling->left&&sibling->left->color==RED)||(sibling->right&&sibling->right->color==RED)){
					if((sibling->left&&sibling->left->color==RED)){
						if(sibling->par->left==sibling){
							sibling->left->color=sibling->color;
							sibling->color=parent->color;
							rightRotate(p,parent);
						}
						else{
							sibling->left->color=parent->color;
							rightRotate(p,sibling);
							leftRotate(p,parent);
						}
					}
					else{
						if(sibling->par->left==sibling){
							sibling->right->color=parent->color;
							leftRotate(p,sibling);
							rightRotate(p,parent);
						}
						else{
							sibling->right->color=sibling->color;
							sibling->color=parent->color;
							leftRotate(p,parent);
						}
					}
					parent->color=BLACK;
				}
				else{
					sibling->color=RED;
					if(parent->color==BLACK){
						fixDBAnamoly(p,parent);
					}
					else{
						parent->color=BLACK;
					}
				}
			}
		}
	}

	/**
     * @brief function to perform inorder traversal
     **************************************/
	void traverse(statusNode*p){
    	if(p==NULL){
    		return;
    	}
    	traverse(p->left);
    	cout<<p->data.segId<<endl;
    	traverse(p->right);

    }

    /**
     * @brief entry-function to perform inorder traversal
     **************************************/
    void inorder(){
    	traverse(p);
    }

    /**
     * @brief function to swap the contents of 2 nodes in binary tree
     **************************************/
    void swap_nodes(Segment s1,Segment s2){
    	statusNode* n1 = get_node(p,s1);
    	statusNode* n2 = get_node(p,s2);
    	swap(n1->data,n2->data);
    	swap(n1->color,n2->color);
    }


};

/**
 * @brief function to indicate if 3 points are collinear
 **************************************/
bool onSeg(point l,point m,point r){
	double mx_x = max(l.x,r.x);
	double mn_x = min(l.x,r.x);
	double mx_y = max(l.y,r.y);
	double mn_y = min(l.y,r.y);
	return ((m.x<=mx_x && m.x>=mn_x) && (m.y<=mx_y && m.y>=mn_y));
}

/**
 * @brief function to indicate if 3 orientation of points.
 **************************************/
int find_orientation(point l,point m,point r){
	double orient = (m.y-l.y)*(r.x-m.x) - (m.x-l.x)*(r.y-m.y);
	if(orient>0){
		return 1;
	}
	else if(orient<0){
		return 2;
	}
	else if(orient==0){
		return 0;
	}
	return -1;
}

/**
 * @brief function to check if 2 line segments intersect.
 **************************************/
bool check_intersection(Segment s1,Segment s2){
	point ps1 = s1.p;
	point qs1 = s1.q;
	point ps2 = s2.p;
	point qs2 = s2.q;
	int orient_1 = find_orientation(ps1,qs1,ps2);
	int orient_2 = find_orientation(ps1,qs1,qs2);
	int orient_3 = find_orientation(ps2,qs2,ps1);
	int orient_4 = find_orientation(ps2,qs2,qs1);

	if(orient_1!=orient_2 && orient_4!=orient_3){
		return true;
	}

	if(orient_1==0&&onSeg(ps1,ps2,qs1)){
		return true;
	}
	if(orient_2==0&&onSeg(ps1,qs2,qs1)){
		return true;
	}
	if(orient_3==0&&onSeg(ps2,ps1,qs2)){
		return true;
	}
	if(orient_4==0&&onSeg(ps2,qs1,qs2)){
		return true;
	}
	return false;

}

/**
 * @brief function to get point of intersection of 2 line segments
 **************************************/
point get_intersection(Segment s1,Segment s2){
	double x1 = s1.p.x;
	double y1 = s1.p.y;
	double x2 = s1.q.x;
	double y2 = s1.q.y;
	double a1 = s2.p.x;
	double b1 = s2.p.y;
	double a2 = s2.q.x;
	double b2 = s2.q.y;
    double m1 = (y2-y1)/(x2-x1);
    double m2 = (b2-b1)/(a2-a1);
	double x = (b1 - y1 + m1*x1 - m2*a1)/(m1-m2);
	double y = m1*x + y1 - m1*x1;
	point in = point(x,y,s1.segId,s2.segId,0);
	return in;
}

bool comp(const Segment&a,const Segment&b){
	return a.p.x<b.p.x;
}



/**
 * @brief Implementation of the line-sweep algorithm to detect and find intersection points among pairs of line segments 
 **************************************/
int main(){
	vector<point> intersections;/// vector to store intersection points
	vector<Segment> segments; /// vector to store segments
	ifstream fin; /// input file stream
	ofstream fout; /// output file stream
	fin.open("C:/Users/BITS-PC/Desktop/DAA Assignment/Line intersection/TestCases/tc_2.txt");
	fout.open("C:/Users/BITS-PC/Desktop/DAA Assignment/Line intersection/TestCases/tc_2_op.txt");
	string line;
	int id=0;
	while(getline(fin,line)){
		int i=0;
		std::vector<double> pt(4);
		stringstream ss(line);
		string str;
		while(getline(ss,str,' ')){
			pt[i]=stod(str);
			i++;
		}
		point startpt = point(pt[0],pt[1],id,lft);
		point endpt = point(pt[2],pt[3],id,rt);
		Segment s = Segment(startpt,endpt,i);
		segments.push_back(s);
		id++;
	}
	int n = segments.size();
	sort(segments.begin(),segments.end(),comp);
	for(int i=0;i<n;i++){
		segments[i].segId=i;
		segments[i].p.segId=i;
		segments[i].q.segId=i;
		
	}
	eventQueue pts;
	for(int i=0;i<n;i++){
		cout<<"("<<segments[i].p.x<<","<<segments[i].p.y<<")"<<"::"<<"("<<segments[i].q.x<<","<<segments[i].q.y<<")"<<"===>"<<segments[i].segId<<endl;
		cout<<segments[i].p.segId<<"||"<<segments[i].q.segId<<endl;
		pts.balancedInsert(segments[i].p);
		pts.balancedInsert(segments[i].q);
	}
	pts.inorder();
	cout<<"==============="<<endl;

	sweepLineStatus lines;
	eventNode* root = pts.getRoot();
    stack<eventNode*>st;
    int flg=1;

    /// iterative inorder traversal of eventQueue to process event points in order
    while(flg){
    	if(root==NULL){
    		if(st.size()){
    			root=st.top();
                cout<<st.top()->data.x<<","<<st.top()->data.y<<"==>";
                if(root->data.loc==-1){
                	///left endpoint
                	cout<<"left"<<":"<<root->data.segId<<endl;
                	Segment curr = segments[root->data.segId];
                    lines.balancedInsert(curr);
                    int upper_id = lines.get_above_segment(curr);
                    int lower_id = lines.get_below_segment(curr);
                    if(upper_id!=-1){
                    	Segment upper_neighbour = segments[upper_id];
                    	if(check_intersection(curr,upper_neighbour)){
                    		point in = get_intersection(curr,upper_neighbour);
                    		intersections.push_back(in);
                    		pts.balancedInsert(in);
                    	}

                    }
                    if(lower_id!=-1){
                    	Segment lower_neighbour = segments[lower_id];
                    	if(check_intersection(curr,lower_neighbour)){
                    		point in = get_intersection(curr,lower_neighbour);
                    		intersections.push_back(in);
                    		pts.balancedInsert(in);
                    	}
                    }

                }
                else if(root->data.loc==1){
                	/// right endpoint
                	cout<<"right"<<":"<<root->data.segId<<endl;
                	Segment curr = segments[root->data.segId];
                    int upper_id = lines.get_above_segment(curr);
                    int lower_id = lines.get_below_segment(curr);
                    if(upper_id!=-1&&lower_id!=-1){
                    	Segment upper_neighbour = segments[upper_id];
                    	Segment lower_neighbour = segments[lower_id];
                    	if(check_intersection(lower_neighbour,upper_neighbour)){
                    		point in = get_intersection(lower_neighbour,upper_neighbour);
                    		if(pts.search(in)==0){
                    			intersections.push_back(in);
                    			pts.balancedInsert(in);
                    		}
                    	}
                    }
                    lines.delete_node(curr);
                }
                else{ 
                	/// intersection point
                	cout<<"intersection"<<":"<<"{"<<root->data.segId<<","<<root->data.segId1<<"}"<<endl;
                
                	lines.swap_nodes(segments[root->data.segId],segments[root->data.segId1]);
                	int up_seg = lines.get_above_segment(segments[root->data.segId]);
                    int down_seg = lines.get_below_segment(segments[root->data.segId]);
                	int up_seg1 = lines.get_above_segment(segments[root->data.segId1]);
                	int down_seg1 = lines.get_below_segment(segments[root->data.segId1]);
                	if(down_seg==root->data.segId1){
                		/// seg is above seg1
                		if(up_seg!=-1){
                			Segment upper_neighbour = segments[up_seg];
                    	    if(check_intersection(segments[root->data.segId],upper_neighbour)){
                    		   point in = get_intersection(segments[root->data.segId],upper_neighbour);
                    		   intersections.push_back(in);
                    		   pts.balancedInsert(in);
                    	    }
                		}
                		if(down_seg1!=-1){
                			Segment lower_neighbour = segments[down_seg1];
                    	    if(check_intersection(segments[root->data.segId1],lower_neighbour)){
                    		   point in = get_intersection(segments[root->data.segId1],lower_neighbour);
                    		   intersections.push_back(in);
                    		   pts.balancedInsert(in);
                    	    }
                		}
                	}
                	else if(down_seg1==root->data.segId){
                		/// seg1 above seg
                		if(up_seg1!=-1){
                			Segment upper_neighbour = segments[up_seg1];
                    	    if(check_intersection(segments[root->data.segId1],upper_neighbour)){
                    		   point in = get_intersection(segments[root->data.segId1],upper_neighbour);
                    		   intersections.push_back(in);
                    		   pts.balancedInsert(in);
                    	    }
                		}
                		if(down_seg!=-1){
                			Segment lower_neighbour = segments[down_seg];
                    	    if(check_intersection(segments[root->data.segId],lower_neighbour)){
                    		   point in = get_intersection(segments[root->data.segId],lower_neighbour);
                    		   intersections.push_back(in);
                    		   pts.balancedInsert(in);
                    	    }
                		}

                	}

                }
                lines.inorder();
                cout<<endl;
                cout<<"==============="<<endl;
    			st.pop();
    			root=root->right;
    		}
    		else{
    			flg=0;
    		}

    	}
    	else{
    		st.push(root);
    		root=root->left;
    	}

    }
    

    /// Write the set of intersection points in output file
    fout<<"Intersection points"<<endl;
    for(int i=0;i<intersections.size();i++){
    	fout<<"Point of intersection : "<<"("<<intersections[i].x<<","<<intersections[i].y<<") "<<"between segment "<<intersections[i].segId<<" and segment "<<intersections[i].segId1<<endl;
    }
	
	return 0;


}
