#ifndef _MAPEDGE_H
#define _MAPEDGE_H

//图的数据结构
//add by freeyes

#include <stdio.h>
#include <stdlib.h>
#include <iostream>  
#include <string>  
#include <queue>  
using namespace std;  

#define MAXVEX 100

enum EM_LANGUAGE
{
	LANGUAGE_NONE     = -1,
	LITTLE_LANGUAGE_1 = 0,
	LITTLE_LANGUAGE_2,
	CHINESE,
	ENGLISH,
	GERMAN,
	FINANCE,
};

typedef vector<string> vecUserName;
typedef int EdgeWeight;  
struct EdgeNode    
{  
	EM_LANGUAGE      adjvex;    //邻接点  
	EdgeWeight       weight;    //权值
	vecUserName      userlist;  //用户列表
	struct EdgeNode* next;      //指向下一条边  

	EdgeNode()
	{
		adjvex = LANGUAGE_NONE;
		next   = NULL;
		weight = 1;
	}
};  

typedef string VertexType;      //顶点类型  
struct VertexNode  
{  
	EM_LANGUAGE  VertexName;    //点名称 
	vecUserName  userlist;      //用户列表
	EdgeNode*    pFirstEdge;    //指示第一条边  

	VertexNode()
	{
		VertexName = LANGUAGE_NONE;
		pFirstEdge = NULL;
	}

	~VertexNode()
	{
		//清理所有的边
		EdgeNode* pCurr = pFirstEdge;
		while(pCurr != NULL)
		{
			EdgeNode* pnext = pCurr->next;
			delete pCurr;
			pCurr = pnext;
		}
	}
};  

typedef VertexNode AdjList[MAXVEX]; //邻接表  
typedef struct    
{  
	AdjList adjList;         //邻接表  
	int iVexNum;             //顶点个数  
}AdjListGraph;  


//节点距离信息
#define MAX_DISTANCE_LENGTH   999
struct NodeDistance
{
	EM_LANGUAGE  m_emID;
	int          m_nDistonace;
	bool         m_blFind;
	EM_LANGUAGE  m_emBegin;
	EM_LANGUAGE  m_emEnd;

	NodeDistance()
	{
		m_emID       = LANGUAGE_NONE;
		m_emBegin    = LANGUAGE_NONE;
		m_emEnd      = LANGUAGE_NONE;
		m_nDistonace = MAX_DISTANCE_LENGTH;  //初始值无穷大
		m_blFind     = false;                //是否是遍历过的最短的点
	}
};

//路径信息
struct NodePathInfo
{
	vecUserName m_vecUserName;
	EM_LANGUAGE m_emBegin;
	EM_LANGUAGE m_emEnd;

	NodePathInfo()
	{
		m_emBegin     = LANGUAGE_NONE;
		m_emEnd       = LANGUAGE_NONE;
	}
};

typedef vector<NodePathInfo> vecNodeList;


#endif
