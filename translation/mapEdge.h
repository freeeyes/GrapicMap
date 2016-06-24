#ifndef _MAPEDGE_H
#define _MAPEDGE_H

//ͼ�����ݽṹ
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
	EM_LANGUAGE      adjvex;    //�ڽӵ�  
	EdgeWeight       weight;    //Ȩֵ
	vecUserName      userlist;  //�û��б�
	struct EdgeNode* next;      //ָ����һ����  

	EdgeNode()
	{
		adjvex = LANGUAGE_NONE;
		next   = NULL;
		weight = 1;
	}
};  

typedef string VertexType;      //��������  
struct VertexNode  
{  
	EM_LANGUAGE  VertexName;    //������ 
	vecUserName  userlist;      //�û��б�
	EdgeNode*    pFirstEdge;    //ָʾ��һ����  

	VertexNode()
	{
		VertexName = LANGUAGE_NONE;
		pFirstEdge = NULL;
	}

	~VertexNode()
	{
		//�������еı�
		EdgeNode* pCurr = pFirstEdge;
		while(pCurr != NULL)
		{
			EdgeNode* pnext = pCurr->next;
			delete pCurr;
			pCurr = pnext;
		}
	}
};  

typedef VertexNode AdjList[MAXVEX]; //�ڽӱ�  
typedef struct    
{  
	AdjList adjList;         //�ڽӱ�  
	int iVexNum;             //�������  
}AdjListGraph;  


//�ڵ������Ϣ
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
		m_nDistonace = MAX_DISTANCE_LENGTH;  //��ʼֵ�����
		m_blFind     = false;                //�Ƿ��Ǳ���������̵ĵ�
	}
};

//·����Ϣ
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
