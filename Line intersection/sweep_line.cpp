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
		return ans;
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
	Segment data; // y-coordinate of intersection point of sweep line with segment at event pts
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

class sweepLineStatus{
	statusNode*p;
public:
	sweepLineStatus(){
		p=NULL;
	}
	statusNode*getRoot(){
		return p;
	}
	bool isBelow(point a,Segment s){
		int a1 = s.p.y - s.q.y;
		int b1 = s.q.x - s.p.x;
		int c1 = s.p.x*s.q.y - s.q.x*s.p.y;
		int x1 = a.x;
		int y1 = a.y;
		return (a1*x1+b1*y1+c1<0 and b1>0) or (a1*x1+b1*y1+c1>0 and b1<0);
	}
	bool isAbove(point a,Segment s){
		int a1 = s.p.y - s.q.y;
		int b1 = s.q.x - s.p.x;
		int c1 = s.p.x*s.q.y - s.q.x*s.p.y;
		int x1 = a.x;
		int y1 = a.y;
		return (a1*x1+b1*y1+c1>0 and b1>0) or (a1*x1+b1*y1+c1<0 and b1<0);
	}

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

    void fixRedParentAnamoly(statusNode*&p,statusNode*&q){

    	statusNode* parent = NULL,*grandparent=NULL;
    	// Violation would occur if parent node is red 
    	while((q!=p)&&(q->color==RED)&&(q->par->color==RED)){
    		parent = q->par;
    		grandparent = q->par->par;
    		/* Case 1 : parent is left child of grandparent */
    		if(parent==grandparent->left){
    			statusNode* uncle = grandparent->right; // sibling of parent
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
    			statusNode* uncle= grandparent->left;
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

	void balancedInsert(Segment & data){
		statusNode*q = new statusNode(data);
		p = insert(p,q);
		fixRedParentAnamoly(p,q);
	}
	bool bbst_search(statusNode*p,Segment data){
    	if(p==NULL){
    		return false;
    	}
    	if(p->data.segId==data.segId){
    		return true;
    	}
    	return bbst_search(p->left,data)||bbst_search(p->right,data);
    }
	bool search(Segment data){
		bool ans = bbst_search(p,data);
		return ans;
	}
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
	/*int above_segment(point data){
		statusNode*req = p;
		int ans=-1;
		while(req!=NULL){
			if(isBelow(data,p->data)){
				req=req->right;
			}
			else{
				ans = req->data.segId;
				req=req->left;
			}
		}
	    return ans;
	}
	int below_segment(point data){
		statusNode*req = p;
		int ans=-1;
		while(req!=NULL){
			if(isAbove(data,p->data)){
				req=req->left;
			}
			else{
				ans = req->data.segId;
				req=req->right;
			}
		}
	    return ans;
	}*/

	statusNode*successor(statusNode*target){
		statusNode*x=target;
		while(x->left){
			x=x->left;
		}
		return x;
	}

	void delete_node(Segment data){
		statusNode*p=getRoot();
		if(p==NULL){
			return;
		}
		statusNode* target = get_node(p,data);
		balancedRemove(p,target);
	}

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
	void traverse(statusNode*p){
    	if(p==NULL){
    		return;
    	}
    	traverse(p->left);
    	cout<<p->data.segId<<endl;
    	traverse(p->right);

    }
    void inorder(){
    	traverse(p);
    }


};


point get_intersection(Segment s1,Segment s2){




}


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
    cout<<"==============="<<endl;
	sweepLineStatus lines;
	point a1 = point(1,2,0,start);
	point b1 = point(8,1,1,end);
	point c1 = point(2,5,2,start);
	point d1 = point(4,2,3,end);
	point e1 = point(72,2,4,start);
	point f1 = point(71,3,5,start);
	point g1 = point(5,5,6,start);
	point h1 = point(92,2122,7,start);
	point i1 = point(82,21,8,start);

    Segment s1 = Segment(c1,d1,1);
    Segment s2 = Segment(a1,b1,0);
    lines.balancedInsert(s1);
    lines.balancedInsert(s2);
    lines.delete_node(s1);
       	//cout<<"======"<<endl;
    lines.inorder();







	return 0;


}
