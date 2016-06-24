// translation.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "mapEdge.h"

using namespace std;

typedef vector<EM_LANGUAGE> vecLanguage;

//人员信息
struct _UserInfo
{
	string      m_strName;
	vecLanguage m_vecLanguage;
};

//初始化所有语言顶点
void Init_Graphic(AdjListGraph* pAdjListGraph, int nCount)
{
	//一共5中语言
	pAdjListGraph->iVexNum = nCount;
	pAdjListGraph->adjList[0].VertexName = LITTLE_LANGUAGE_1;
	pAdjListGraph->adjList[1].VertexName = LITTLE_LANGUAGE_2;
	pAdjListGraph->adjList[2].VertexName = CHINESE;
	pAdjListGraph->adjList[3].VertexName = ENGLISH;
	pAdjListGraph->adjList[4].VertexName = GERMAN;
}

//检查边是否存在
EdgeNode* Check_Edge_Exist(EdgeNode* pFirstEdge, EM_LANGUAGE emEndNodeID)
{
	if(NULL == pFirstEdge)
	{
		return false;
	}

	EdgeNode* pCurr = pFirstEdge;
	while(pCurr != NULL)
	{
		if(pCurr->adjvex == emEndNodeID)
		{
			return pCurr;
		}

		pCurr = pCurr->next;
	}

	return NULL;
}

//检查边是否存在
void Add_New_Edge(EdgeNode*& pFirstEdge, EdgeNode* pNewEdge)
{
	if(NULL == pFirstEdge)
	{
		pFirstEdge = pNewEdge;
		//return;
	}

	EdgeNode* pCurr = pFirstEdge;
	while(pCurr != NULL)
	{
		if(NULL == pCurr->next)
		{
			pCurr->next       = pNewEdge;
			pCurr->next->next = NULL;
			return;
		}

		pCurr = pCurr->next;
	}
}

//将用户所会语言添加到语言图中
void Add_Edge_Graphic(AdjListGraph* pAdjListGraph, _UserInfo* pUserInfo)
{
	//将语言顶点加入用户信息
	for(int i = 0; i < (int)pUserInfo->m_vecLanguage.size(); i++)
	{
		EM_LANGUAGE emLanuage = pUserInfo->m_vecLanguage[i];
		pAdjListGraph->adjList[emLanuage].userlist.push_back(pUserInfo->m_strName);
	}

	//开始遍历边，如果用户会多种语言，建立边关系
	if(pUserInfo->m_vecLanguage.size() >= 2)
	{
		for(int i = 0; i < (int)pUserInfo->m_vecLanguage.size(); i++)
		{
			for(int j = i + 1; j < (int)pUserInfo->m_vecLanguage.size(); j++)
			{
				//由于是无向图，任何一个边都需要画两次
				EM_LANGUAGE emBageinLanuage = pUserInfo->m_vecLanguage[i];
				EM_LANGUAGE emEndLanuage    = pUserInfo->m_vecLanguage[j];
				//检查当前边是否存在
				EdgeNode* pCurrEdge = Check_Edge_Exist(
					pAdjListGraph->adjList[emBageinLanuage].pFirstEdge,
					emEndLanuage);
				if(NULL == pCurrEdge)
				{
					//如果边不存在，创建一个新的边。并添加这个用户
					pCurrEdge = new EdgeNode();
					pCurrEdge->adjvex = emEndLanuage;
					pCurrEdge->userlist.push_back(pUserInfo->m_strName);
					Add_New_Edge(pAdjListGraph->adjList[emBageinLanuage].pFirstEdge, pCurrEdge);
				}
				else
				{
					//如果这个边已存在，并添加这个用户
					pCurrEdge->userlist.push_back(pUserInfo->m_strName);
				}

				emBageinLanuage = pUserInfo->m_vecLanguage[j];
				emEndLanuage    = pUserInfo->m_vecLanguage[i];
				pCurrEdge = Check_Edge_Exist(
					pAdjListGraph->adjList[emBageinLanuage].pFirstEdge,
					emEndLanuage);
				if(NULL == pCurrEdge)
				{
					//如果边不存在，创建一个新的边。并添加这个用户
					pCurrEdge = new EdgeNode();
					pCurrEdge->adjvex = emEndLanuage;
					pCurrEdge->userlist.push_back(pUserInfo->m_strName);
					Add_New_Edge(pAdjListGraph->adjList[emBageinLanuage].pFirstEdge, pCurrEdge);
				}
				else
				{
					//如果这个边已存在，并添加这个用户
					//如果这个边已存在，并添加这个用户
					pCurrEdge->userlist.push_back(pUserInfo->m_strName);
				}
			}
		}
	}
}

bool Check_All_Path(NodeDistance* pDisanceList, int nCount)
{
	for(int i = 0;i < nCount; i++)
	{
		if(pDisanceList[i].m_blFind == false)
		{
			return true;
		}
	}

	return false;
}

EM_LANGUAGE Find_Min_Node(NodeDistance* pDisanceList, int nCount)
{
	EM_LANGUAGE emMinNode  = LANGUAGE_NONE;
	int         nMinWeigth = MAX_DISTANCE_LENGTH;
	for(int i = 0;i < nCount; i++)
	{
		if(pDisanceList[i].m_blFind == false && nMinWeigth > pDisanceList[i].m_nDistonace)
		{
			nMinWeigth = pDisanceList[i].m_nDistonace;
			emMinNode  = pDisanceList[i].m_emID;
			//objList.push_back(pDisanceList[i].m_emID);
		}
	}

	return emMinNode;
}

void Get_Node_User(AdjListGraph* pAdjListGraph, EM_LANGUAGE emBegin, EM_LANGUAGE emEnd, vecUserName& vecUserName)
{
	string strUserName = "";
	EdgeNode* pCurrEdge = pAdjListGraph->adjList[emBegin].pFirstEdge;
	while(pCurrEdge != NULL)
	{
		if(pCurrEdge->adjvex == emEnd)
		{
			for(int i = 0; i < pCurrEdge->userlist.size(); i++)
			{
				vecUserName.push_back(pCurrEdge->userlist[i]);
			}
			
			break;
		}
		pCurrEdge = pCurrEdge->next;
	}
}

void Add_Path_Info(vecNodeList& objvecUserList, vecNodeList& objTempPathList, NodePathInfo objNodePathInfo)
{
	//遍历这个列表。因为存在分支走不通重新走的情况
	//所以获得每个运行节点，都需要判断是否和上一个点是连续的
	bool blIsFind = false;
	int  nPos     = 0;
	for(int i = 0; i < (int)objvecUserList.size(); i++)
	{
		if(objvecUserList[i].m_emEnd == objNodePathInfo.m_emBegin)
		{
			blIsFind = true;
			nPos     = i;
			break;
		}
	}

	if(blIsFind == false)
	{
		if(objvecUserList.size() == 0)
		{
			//没有找到节点，说明是第一个数据
			objvecUserList.push_back(objNodePathInfo);
		}
		else
		{
			//在过去淘汰的节点中寻找
			if(objTempPathList.size() > 0)
			{
				//删除当前最后一个节点
				for(vecNodeList::iterator b = objvecUserList.begin() + objvecUserList.size() - 1; b != objvecUserList.end();)
				{
					b = objvecUserList.erase(b);
				}

				bool blIsTemp = false;
				int  nTempPos = 0;
				for(int i = 0 ; i < (int)objTempPathList.size(); i++)
				{
					if(objTempPathList[i].m_emEnd = objNodePathInfo.m_emBegin)
					{
						blIsTemp = true;
						nTempPos = i;
						break;
					}
				}

				if(true == blIsTemp)
				{
					objvecUserList.push_back(objTempPathList[nTempPos]);
					objvecUserList.push_back(objNodePathInfo);

					objTempPathList.erase(objTempPathList.begin() + nTempPos);
				}
			}

		}
	}
	else
	{
		if(nPos == (int)objvecUserList.size() - 1)
		{
			//是最后一个节点
			objvecUserList.push_back(objNodePathInfo);
		}
		else
		{
			//不是最后一个节点，找到那个节点，之后的路径全部删除，添加新节点
			for(vecNodeList::iterator b = objvecUserList.begin() + nPos + 1; b != objvecUserList.end();)
			{
				objTempPathList.push_back((*b));
				b = objvecUserList.erase(b);
			}

			//添加新元素
			objvecUserList.push_back(objNodePathInfo);
		}
	}
}

//得到从一种语言到另一种语言的传递用户过程
//Dijkstra算法，贪心算法
bool Get_Path_User(EM_LANGUAGE emBegin, EM_LANGUAGE emEnd, AdjListGraph* pAdjListGraph, vecNodeList& vecUserList)
{
	vecNodeList objTemnpList;   //淘汰列表
	vecUserList.clear();
	NodeDistance* pDisanceList = new NodeDistance[pAdjListGraph->iVexNum];

	//得到起始定点
	EdgeNode* pCurrEdge  = pAdjListGraph->adjList[emBegin].pFirstEdge;
	int       nCurrWigth = 0;

	//第一个点不必再遍历
	pDisanceList[emBegin].m_emID       = emBegin;
	pDisanceList[emBegin].m_nDistonace = 0;
	pDisanceList[emBegin].m_blFind     = true;

	EM_LANGUAGE nPrevNode = LANGUAGE_NONE;
	EM_LANGUAGE nNextNode = emBegin;

	while(Check_All_Path(pDisanceList, pAdjListGraph->iVexNum))
	{
		pCurrEdge = pAdjListGraph->adjList[nNextNode].pFirstEdge;

		//遍历所有当前点到下一个点边
		while(pCurrEdge != NULL)
		{
			if(pDisanceList[pCurrEdge->adjvex].m_nDistonace == MAX_DISTANCE_LENGTH)
			{
				pDisanceList[pCurrEdge->adjvex].m_emID       = pCurrEdge->adjvex;
				pDisanceList[pCurrEdge->adjvex].m_nDistonace = pCurrEdge->weight + nCurrWigth;
				pDisanceList[pCurrEdge->adjvex].m_emBegin    = nNextNode;
				pDisanceList[pCurrEdge->adjvex].m_emEnd      = pCurrEdge->adjvex;
			}
			else
			{
				if(pDisanceList[pCurrEdge->adjvex].m_blFind != true)
				{
					pDisanceList[pCurrEdge->adjvex].m_nDistonace = pCurrEdge->weight + nCurrWigth;
					pDisanceList[pCurrEdge->adjvex].m_emBegin    = nNextNode;
					pDisanceList[pCurrEdge->adjvex].m_emEnd      = pCurrEdge->adjvex;
				}
			}

			pCurrEdge = pCurrEdge->next;
		}

		//记录上一个节点
		nPrevNode = nNextNode;

		nNextNode = Find_Min_Node(pDisanceList, pAdjListGraph->iVexNum);
		if (nNextNode != LANGUAGE_NONE)
		{
			pDisanceList[nNextNode].m_blFind = true;
			nCurrWigth = pDisanceList[nNextNode].m_nDistonace;

			if(nNextNode == emEnd)
			{
				//已经找到了路径
				NodePathInfo objNodePathInfo;
				Get_Node_User(pAdjListGraph, pDisanceList[nNextNode].m_emBegin, pDisanceList[nNextNode].m_emEnd, objNodePathInfo.m_vecUserName);
				objNodePathInfo.m_emBegin     = pDisanceList[nNextNode].m_emBegin;
				objNodePathInfo.m_emEnd       = pDisanceList[nNextNode].m_emEnd;
				Add_Path_Info(vecUserList, objTemnpList, objNodePathInfo);
				delete[] pDisanceList;
				return true;
			}
			else
			{
				NodePathInfo objNodePathInfo;
				Get_Node_User(pAdjListGraph, pDisanceList[nNextNode].m_emBegin, pDisanceList[nNextNode].m_emEnd, objNodePathInfo.m_vecUserName);
				objNodePathInfo.m_emBegin     = pDisanceList[nNextNode].m_emBegin;
				objNodePathInfo.m_emEnd       = pDisanceList[nNextNode].m_emEnd;
				Add_Path_Info(vecUserList, objTemnpList, objNodePathInfo);
			}
		}
		else
		{
			//全部路径查找完结
			vecUserList.clear();
			delete[] pDisanceList;
			return false;
		}
	}

	vecUserList.clear();
	delete[] pDisanceList;
	return false;
}

int _tmain(int argc, _TCHAR* argv[])
{
	//创建人员
	_UserInfo objA;
	_UserInfo objB;
	_UserInfo objC;
	_UserInfo objD;
	_UserInfo objE;

	//页面信息
	objA.m_strName   = "A";
	objB.m_strName   = "B";
	objC.m_strName   = "C";
	objD.m_strName   = "D";
	objE.m_strName   = "E";

	//人员设置
	objA.m_vecLanguage.push_back(LITTLE_LANGUAGE_1);
	objA.m_vecLanguage.push_back(LITTLE_LANGUAGE_2);

	objB.m_vecLanguage.push_back(LITTLE_LANGUAGE_2);
	objB.m_vecLanguage.push_back(CHINESE);
	
	objC.m_vecLanguage.push_back(LITTLE_LANGUAGE_2);
	objC.m_vecLanguage.push_back(CHINESE);
	objC.m_vecLanguage.push_back(ENGLISH);
	
	objD.m_vecLanguage.push_back(ENGLISH);
	objD.m_vecLanguage.push_back(GERMAN);
	objD.m_vecLanguage.push_back(FINANCE);
	
	objE.m_vecLanguage.push_back(ENGLISH);
	objE.m_vecLanguage.push_back(GERMAN);
	objE.m_vecLanguage.push_back(FINANCE);

	//创建图
	AdjListGraph objAdjListGraph;

	//初始化图的信息
	Init_Graphic(&objAdjListGraph, 6);

	//将人员填入图
	Add_Edge_Graphic(&objAdjListGraph, &objA);
	Add_Edge_Graphic(&objAdjListGraph, &objB);
	Add_Edge_Graphic(&objAdjListGraph, &objC);
	Add_Edge_Graphic(&objAdjListGraph, &objD);
	Add_Edge_Graphic(&objAdjListGraph, &objE);

	//寻找任意两点间的路径
	vecNodeList vecUserList;
	bool blState = Get_Path_User(FINANCE, LITTLE_LANGUAGE_1, &objAdjListGraph, vecUserList);
	if(true == blState)
	{
		for(int i = 0 ; i < (int)vecUserList.size(); i++)
		{
			printf_s("[Main]Translation(%d ---> %d).(", vecUserList[i].m_emBegin, vecUserList[i].m_emEnd);
			for(int j = 0; j < (int)vecUserList[i].m_vecUserName.size(); j++)
			{
				if(j == 0)
				{
					printf_s("%s", vecUserList[i].m_vecUserName[j].c_str());
				}
				else
				{
					printf_s(",%s", vecUserList[i].m_vecUserName[j].c_str());
				}
			}
			
			printf_s(")\n");
		}
	}
	else
	{
		printf_s("[Main]No find path.\n");
	}


	getchar();

	return 0;
}

