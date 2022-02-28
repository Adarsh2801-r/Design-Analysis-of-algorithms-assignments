#include <iostream> 
#include<bits/stdc++.h>

#define start -1
#define end 1
#define RED 2
#define BLACK 3

using namespace std;



struct point{
	double x,y;
	int segId=-1;
	int loc=-1;
	point(){
		x=0;
		y=0;
		segId=-1;
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
};

struct Segment{
	point p,q;//p = left , q = right
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

class eventQueue{
	eventNode*p;
public:
	eventQueue(){
		p=NULL;
	}

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

    void fixRedParentAnamoly(eventNode*&p,eventNode*&q){

    	eventNode* parent = NULL,*grandparent=NULL;
    	// Violation would occur if parent node is red 
    	while((q!=p)&&(q->color==RED)&&(q->par->color==RED)){
    		parent = q->par;
    		grandparent = q->par->par;
    		/* Case 1 : parent is left child of grandparent */
    		if(parent==grandparent->left){
    			eventNode* uncle = grandparent->right; // sibling of parent
    			 /* Case 1.1 : uncle node is red : push the violation upward */
    			 if(uncle&&uncle->color==RED){
    			 	parent->color=BLACK;
    			 	uncle->color=BLACK;
    			 	grandparent->color=RED;
    			 	q = grandparent;
    			 }
    			 /* Case 1.2 : uncle node is black */
    			 else{
    			 	/* Case 1.2.1 */
    			 	if(q==parent->right){
    			 		leftRotate(p,parent);
    			 		q=parent;
    			 		parent=q->par;
    			 	}
    			 	/* Case 1.2.2 */
                    swap(parent->color,grandparent->color);
                    rightRotate(p,grandparent);
                    q = parent;

    			 }


    		}
    		/* Case 2 : parent is right child of grandparent */
    		else
    		{
    			eventNode* uncle= grandparent->left;
    			/*Case 2.1*/
    			if(uncle&&uncle->color==RED){
    			 	parent->color=BLACK;
    			 	uncle->color=BLACK;
    			 	grandparent->color=RED;
    			 	q = grandparent;
    			 }
    			 /*Case 2.2*/
    			 else{
    			 	/* Case 2.2.1 */
    			 	if(q==parent->left){
    			 		rightRotate(p,parent);
    			 		q=parent;
    			 		parent=q->par;
    			 	}
    			 	/* Case 2.2.2 */
                    swap(parent->color,grandparent->color);
                    leftRotate(p,grandparent);
                    q = parent;


    			 }



    		}

    	}

        p->color=BLACK;
    }

	void balancedInsert(point & data){
		eventNode*q = new eventNode(data);
		p = insert(p,q);
		fixRedParentAnamoly(p,q);
	}

    bool bbst_search(eventNode*p,point data){
    	if(p==NULL){
    		return false;
    	}
    	if(p->data.segId==data.segId){
    		return true;
    	}
    	return bbst_search(p->left,data)||bbst_search(p->right,data);
    }
	bool search(point data){
		bool ans = bbst_search(p,data);
	}

    void traverse(eventNode*p){
    	if(p==NULL){
    		return;
    	}
    	traverse(p->left);
    	cout<<"("<<p->data.x<<","<<p->data.y<<")"<<endl;
    	traverse(p->right);

    }
    void inorder(){
    	traverse(p);
    }


};

struct statusNode{
	int data;
	int segId;
	int color;
	statusNode* par;
    statusNode* left;
	statusNode* right;
    statusNode(int y_coordinate, int s){
    	this->data=y_coordinate;
    	par=NULL;
    	left=NULL;
    	right=NULL;
    	this->color=RED; 
    	this->segId=s;
    }
};

class sweepLineStatus{
	sweepLineStatus*p;
public:
	sweepLineStatus(){
		p=NULL;
	}
	statusNode* insert(statusNode*p,statusNode*q){
		if(p==NULL){
			return q;
		}	
		if(((q->data))<((p->data))){
			p->left = insert(p->left,q);
			p->left->par=p;
		}
		else if(((q->data))>=((p->data))){
			p->right = insert(p->right,q);
			p->right->par=p;
		}
		return p;
	}
    
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

    void rightRotate(statusNode*&p,*&q){
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

    void fixRedParentAnamoly(eventNode*&p,eventNode*&q){

    	eventNode* parent = NULL,*grandparent=NULL;
    	// Violation would occur if parent node is red 
    	while((q!=p)&&(q->color==RED)&&(q->par->color==RED)){
    		parent = q->par;
    		grandparent = q->par->par;
    		/* Case 1 : parent is left child of grandparent */
    		if(parent==grandparent->left){
    			eventNode* uncle = grandparent->right; // sibling of parent
    			 /* Case 1.1 : uncle node is red : push the violation upward */
    			 if(uncle&&uncle->color==RED){
    			 	parent->color=BLACK;
    			 	uncle->color=BLACK;
    			 	grandparent->color=RED;
    			 	q = grandparent;
    			 }
    			 /* Case 1.2 : uncle node is black */
    			 else{
    			 	/* Case 1.2.1 */
    			 	if(q==parent->right){
    			 		leftRotate(p,parent);
    			 		q=parent;
    			 		parent=q->par;
    			 	}
    			 	/* Case 1.2.2 */
                    swap(parent->color,grandparent->color);
                    rightRotate(p,grandparent);
                    q = parent;

    			 }


    		}
    		/* Case 2 : parent is right child of grandparent */
    		else
    		{
    			eventNode* uncle= grandparent->left;
    			/*Case 2.1*/
    			if(uncle&&uncle->color==RED){
    			 	parent->color=BLACK;
    			 	uncle->color=BLACK;
    			 	grandparent->color=RED;
    			 	q = grandparent;
    			 }
    			 /*Case 2.2*/
    			 else{
    			 	/* Case 2.2.1 */
    			 	if(q==parent->left){
    			 		rightRotate(p,parent);
    			 		q=parent;
    			 		parent=q->par;
    			 	}
    			 	/* Case 2.2.2 */
                    swap(parent->color,grandparent->color);
                    leftRotate(p,grandparent);
                    q = parent;


    			 }



    		}

    	}

        p->color=BLACK;
    }

	void balancedInsert(point & data){
		eventNode*q = new eventNode(data);
		p = insert(p,q);
		fixRedParentAnamoly(p,q);
	}

};



int main(){
	vector<point> intersections;// to find
	vector<Segment> segments ; //given
	int n = 6;
	/*for(int i=0;i<n;i++){
		int p1,q1,p2,q2;
		cin>>p1>>q1>>p2>>q2;
		point startpt = point(p1,q1,i,start);
		point endpt = point(p2,q2,i,end);
		Segment s = Segment(startpt,endpt,i);
		segments.push_back(s);
	}
	for(int i=0;i<n;i++){
		cout<<"("<<segments[i].p.x<<","<<segments[i].p.y<<")"<<"::"<<"("<<segments[i].q.x<<","<<segments[i].q.y<<")"<<"===>"<<segments[i].segId<<endl;
		cout<<segments[i].p.segId<<"||"<<segments[i].q.segId<<endl;

	}*/



	eventQueue pts;
	point a = point(1,2);
	point b = point(311,1);
	point c = point(41,2);
	point d = point(2,2);
	point e = point(72,2);
	point f = point(71,3);
	point g = point(5,5);
	point h = point(92,2122);
	point i = point(82,21);

	pts.balancedInsert(a);
	pts.balancedInsert(b);
	pts.balancedInsert(c);
	pts.balancedInsert(d);
	pts.balancedInsert(e);
	pts.balancedInsert(f);
	pts.balancedInsert(g);
	pts.balancedInsert(h);
	pts.balancedInsert(i);
	cout<<pts.search(a)<<endl;


	pts.inorder();
	return 0;


}