// translation.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "mapEdge.h"

using namespace std;

typedef vector<EM_LANGUAGE> vecLanguage;

//��Ա��Ϣ
struct _UserInfo
{
	string      m_strName;
	vecLanguage m_vecLanguage;
};

//��ʼ���������Զ���
void Init_Graphic(AdjListGraph* pAdjListGraph, int nCount)
{
	//һ��5������
	pAdjListGraph->iVexNum = nCount;
	pAdjListGraph->adjList[0].VertexName = LITTLE_LANGUAGE_1;
	pAdjListGraph->adjList[1].VertexName = LITTLE_LANGUAGE_2;
	pAdjListGraph->adjList[2].VertexName = CHINESE;
	pAdjListGraph->adjList[3].VertexName = ENGLISH;
	pAdjListGraph->adjList[4].VertexName = GERMAN;
}

//�����Ƿ����
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

//�����Ƿ����
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

//���û�����������ӵ�����ͼ��
void Add_Edge_Graphic(AdjListGraph* pAdjListGraph, _UserInfo* pUserInfo)
{
	//�����Զ�������û���Ϣ
	for(int i = 0; i < (int)pUserInfo->m_vecLanguage.size(); i++)
	{
		EM_LANGUAGE emLanuage = pUserInfo->m_vecLanguage[i];
		pAdjListGraph->adjList[emLanuage].userlist.push_back(pUserInfo->m_strName);
	}

	//��ʼ�����ߣ�����û���������ԣ������߹�ϵ
	if(pUserInfo->m_vecLanguage.size() >= 2)
	{
		for(int i = 0; i < (int)pUserInfo->m_vecLanguage.size(); i++)
		{
			for(int j = i + 1; j < (int)pUserInfo->m_vecLanguage.size(); j++)
			{
				//����������ͼ���κ�һ���߶���Ҫ������
				EM_LANGUAGE emBageinLanuage = pUserInfo->m_vecLanguage[i];
				EM_LANGUAGE emEndLanuage    = pUserInfo->m_vecLanguage[j];
				//��鵱ǰ���Ƿ����
				EdgeNode* pCurrEdge = Check_Edge_Exist(
					pAdjListGraph->adjList[emBageinLanuage].pFirstEdge,
					emEndLanuage);
				if(NULL == pCurrEdge)
				{
					//����߲����ڣ�����һ���µıߡ����������û�
					pCurrEdge = new EdgeNode();
					pCurrEdge->adjvex = emEndLanuage;
					pCurrEdge->userlist.push_back(pUserInfo->m_strName);
					Add_New_Edge(pAdjListGraph->adjList[emBageinLanuage].pFirstEdge, pCurrEdge);
				}
				else
				{
					//���������Ѵ��ڣ����������û�
					pCurrEdge->userlist.push_back(pUserInfo->m_strName);
				}

				emBageinLanuage = pUserInfo->m_vecLanguage[j];
				emEndLanuage    = pUserInfo->m_vecLanguage[i];
				pCurrEdge = Check_Edge_Exist(
					pAdjListGraph->adjList[emBageinLanuage].pFirstEdge,
					emEndLanuage);
				if(NULL == pCurrEdge)
				{
					//����߲����ڣ�����һ���µıߡ����������û�
					pCurrEdge = new EdgeNode();
					pCurrEdge->adjvex = emEndLanuage;
					pCurrEdge->userlist.push_back(pUserInfo->m_strName);
					Add_New_Edge(pAdjListGraph->adjList[emBageinLanuage].pFirstEdge, pCurrEdge);
				}
				else
				{
					//���������Ѵ��ڣ����������û�
					//���������Ѵ��ڣ����������û�
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
	//��������б���Ϊ���ڷ�֧�߲�ͨ�����ߵ����
	//���Ի��ÿ�����нڵ㣬����Ҫ�ж��Ƿ����һ������������
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
			//û���ҵ��ڵ㣬˵���ǵ�һ������
			objvecUserList.push_back(objNodePathInfo);
		}
		else
		{
			//�ڹ�ȥ��̭�Ľڵ���Ѱ��
			if(objTempPathList.size() > 0)
			{
				//ɾ����ǰ���һ���ڵ�
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
			//�����һ���ڵ�
			objvecUserList.push_back(objNodePathInfo);
		}
		else
		{
			//�������һ���ڵ㣬�ҵ��Ǹ��ڵ㣬֮���·��ȫ��ɾ��������½ڵ�
			for(vecNodeList::iterator b = objvecUserList.begin() + nPos + 1; b != objvecUserList.end();)
			{
				objTempPathList.push_back((*b));
				b = objvecUserList.erase(b);
			}

			//�����Ԫ��
			objvecUserList.push_back(objNodePathInfo);
		}
	}
}

//�õ���һ�����Ե���һ�����ԵĴ����û�����
//Dijkstra�㷨��̰���㷨
bool Get_Path_User(EM_LANGUAGE emBegin, EM_LANGUAGE emEnd, AdjListGraph* pAdjListGraph, vecNodeList& vecUserList)
{
	vecNodeList objTemnpList;   //��̭�б�
	vecUserList.clear();
	NodeDistance* pDisanceList = new NodeDistance[pAdjListGraph->iVexNum];

	//�õ���ʼ����
	EdgeNode* pCurrEdge  = pAdjListGraph->adjList[emBegin].pFirstEdge;
	int       nCurrWigth = 0;

	//��һ���㲻���ٱ���
	pDisanceList[emBegin].m_emID       = emBegin;
	pDisanceList[emBegin].m_nDistonace = 0;
	pDisanceList[emBegin].m_blFind     = true;

	EM_LANGUAGE nPrevNode = LANGUAGE_NONE;
	EM_LANGUAGE nNextNode = emBegin;

	while(Check_All_Path(pDisanceList, pAdjListGraph->iVexNum))
	{
		pCurrEdge = pAdjListGraph->adjList[nNextNode].pFirstEdge;

		//�������е�ǰ�㵽��һ�����
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

		//��¼��һ���ڵ�
		nPrevNode = nNextNode;

		nNextNode = Find_Min_Node(pDisanceList, pAdjListGraph->iVexNum);
		if (nNextNode != LANGUAGE_NONE)
		{
			pDisanceList[nNextNode].m_blFind = true;
			nCurrWigth = pDisanceList[nNextNode].m_nDistonace;

			if(nNextNode == emEnd)
			{
				//�Ѿ��ҵ���·��
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
			//ȫ��·���������
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
	//������Ա
	_UserInfo objA;
	_UserInfo objB;
	_UserInfo objC;
	_UserInfo objD;
	_UserInfo objE;

	//ҳ����Ϣ
	objA.m_strName   = "A";
	objB.m_strName   = "B";
	objC.m_strName   = "C";
	objD.m_strName   = "D";
	objE.m_strName   = "E";

	//��Ա����
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

	//����ͼ
	AdjListGraph objAdjListGraph;

	//��ʼ��ͼ����Ϣ
	Init_Graphic(&objAdjListGraph, 6);

	//����Ա����ͼ
	Add_Edge_Graphic(&objAdjListGraph, &objA);
	Add_Edge_Graphic(&objAdjListGraph, &objB);
	Add_Edge_Graphic(&objAdjListGraph, &objC);
	Add_Edge_Graphic(&objAdjListGraph, &objD);
	Add_Edge_Graphic(&objAdjListGraph, &objE);

	//Ѱ������������·��
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

