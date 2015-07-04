// A���㷨�� ʵ��A���㷨���߼�
//////////////////////////////////////////////////////////////////////////////////////////
//1. �����A��ʼ, ������Ϊ������ķ������һ��"�����б�", �����б����һ���ȴ���鷽����б�.
//2. Ѱ�����A��Χ���Ե���ķ���, �����Ƿ���"�����б�", ���������ǵ�"������"ΪA.
//3. ��"�����б�"��ɾ����� A, ������� A ����"�ر��б�", "�ر��б�"�д�ŵĶ��ǲ���Ҫ�ٴμ��ķ���
//4. ������������ڲ��ҿ��Ե��� (�ϰ���� "�ر��б�" �ķ��񶼲�����) �ķ���. �����Щ���񻹲��� "�����б�" ��Ļ�, �����Ǽ��� "�����б�",
//   ������Щ����� G, H �� F ֵ���Ƕ���, ���������ǵ� "������" Ϊ C.
//5. ���ĳ�����ڷ��� D �Ѿ��� "�����б�" ����, ���������µ�·�� (���Ǿ���C ��·��) �������Ļ�, Gֵ�Ƿ�����һЩ, ����µ�Gֵ����,
//   �ǾͰ����� "������" ��ΪĿǰѡ�еķ��� C, Ȼ�����¼������� F ֵ�� G ֵ (H ֵ����Ҫ���¼���, ��Ϊ����ÿ������, H ֵ�ǲ����). 
//   ����µ� G ֵ�Ƚϸ�, ��˵������ C �ٵ��� D ����һ�����ǵ�ѡ��, ��Ϊ����Ҫ��Զ��·, ��ʱ����ʲôҲ����.
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

using namespace std;

// �����Ƿ����� 0���� 1������
enum CAN_GO{ YES = 0, NO = 1 };


// Point��  ��ʾһ����ά����
class CPoint
{
private:
	float posX;
	float posY;
	CPoint *parent;
	float _g;
	float _h;
	float _f;

public:
	CPoint() : posX(0), posY(0), parent(NULL), _g(0), _h(0), _f(0) {}

	CPoint(float x = 0, float y = 0, CPoint *p = NULL) : posX(x), posY(y), parent(p), _g(0), _h(0), _f(0) {}

	CPoint(const CPoint &cp)
	{
		posX = cp.posX;
		posY = cp.posY;
	}

	~CPoint(){}

	void set_g(float g) { _g = g; }
	void set_h(float h) { _h = h; }
	void calc_f() { _f = _g + _h; }
	float get_g() const { return _g; }
	float get_h() const { return _h; }
	float get_f() const { return _f; }
	float getPosX() const { return posX; }
	float getPosY() const { return posY; }
	const CPoint *get_parent() const { return parent; }


	bool operator()(const CPoint &pt) const { return posX == pt.posX && posY == pt.posY; }
	bool operator==(const CPoint& pt) { return posX == pt.posX && posY == pt.posY; }
	bool operator==(CPoint& pt) { return posX == pt.posX && posY == pt.posY; }
	bool operator< (const CPoint &p) { return _f < p.get_f(); }

	friend ostream &operator<<(ostream &os, const CPoint &cp)
	{
		os<<"X:"<<cp.getPosX()<<" Y:"<<cp.getPosY();
		return os;
	}

	//��������پ���
	float getManhattanDistance(const CPoint &pt) { return std::abs(posX - pt.posX) + std::abs(posY - pt.posY); }
	
	float calc_g(const CPoint &pt) 
	{
		float g = 1.0;
		const CPoint *p = pt.get_parent();
		if(p != NULL)
		{
			return g + p->get_g();
		}
		return g;

	}

	// ���ŷ����þ���
	float getEuclidDistance(const CPoint &pt) { return std::sqrt((posX - pt.posX) * (posX - pt.posX) + (posY - pt.posY) * (posY - pt.posY)); };

	bool allRight() { return posX >= 0 && posY >= 0; }
	
	void set_parent(CPoint *p) { parent = p; }
};

struct cmp
{
public:
	cmp(CPoint *cp) : _cp(cp) { }
	bool operator() (const CPoint *cp) { *_cp == *cp; }
private:
	CPoint *_cp;
};

class AStar
{
public:
	AStar(int *pathTbl, CPoint& s, CPoint& e) : pathTable(pathTbl)/*, start(s), end(e)*/ {
		start = new CPoint(s.getPosX(), s.getPosY());
		end = new CPoint(e.getPosX(), e.getPosY());
		pathPlanning();
	}

	~AStar()
	{
		for(size_t i = 0; i < openList.size(); ++i)
		{
			if(openList[i] != NULL)
			{
				delete openList[i];
				openList[i] = NULL;
			}
		}
		for(size_t i = 0; i < closeList.size(); ++i)
		{
			if(closeList[i] != NULL)
			{
				delete closeList[i];
				closeList[i] = NULL;
			}
		}
		delete end;
		openList.clear();
		closeList.clear();
	}
protected:

	void get_arround(vector<CPoint> &v, const CPoint *cur)
	{
		float dir[][2] = {{1.0, 0.0}, {1/2.0, 1.0}, {-1/2.0, 1.0}, {-1.0, 0.0}, {-1/2.0, -1.0}, {1/2.0, -1.0}};
		for (int i = 0; i < 6; i++)
		{
			float x = cur->getPosX() + dir[i][0];
			float y = cur->getPosY() + dir[i][1];
			int row = int(y);
			int col = int(x/2);
			// �����ϰ� ����
			if(*(pathTable+row*3+col) == YES )
			{
				v.push_back(CPoint(x, y));
			}
		}
	}

	//·���滮���㷨�ĺ���
	void pathPlanning()
	{
		// ÿ��·�������ƶ������޸�ֵ
	//	float dir[][2] = {{1, 0}, {1/2, 1}, {-1/2, 1}, {-1, 0}, {-1/2, -1}, {1/2, -1}};
		// ������open�б�
		openList.push_back(start);
		while (!openList.empty())
		{
			CPoint *cur = *openList.begin();
			openList.erase(openList.begin());
			closeList.push_back(cur);
			if (*cur == *end)
			{
				//initPath();
				generate_path(cur);
				return;
			}
			vector<CPoint> v;
			get_arround(v, cur);
			for(vector<CPoint>::iterator it = v.begin(); it != v.end(); ++it)
			{
				// ��closeList��
				if(find_if(closeList.begin(), closeList.end(), cmp(&*it)) != closeList.end())
				{
					continue;
				}
				// ��openList��
				vector<CPoint *>::iterator i = find_if(openList.begin(), openList.end(), cmp(&*it));
				if(i != openList.end())
				{
					float g = (*i)->calc_g(*cur);
					if((*i)->get_g() > g)
					{
						(*i)->set_g(g);
						(*i)->set_parent(cur);
						(*i)->calc_f();
					}
				}
				else // ����openList��
				{
					CPoint *cp = new CPoint(it->getPosX(), it->getPosY());
					cp->set_parent(cur);
					float g = cp->calc_g(*cur);
					cp->set_g(g);
					float h = cp->getEuclidDistance(*end);
					cp->set_h(h);
					cp->calc_f();
					openList.push_back(cp);
				}
			}
			sort(openList.begin(), openList.end());
		}
		std::cout<<"û��·������"<<std::endl;
	}

	void generate_path(const CPoint *cp)
	{
		while(cp)
		{
			pathList.push_back(*cp);
			cp = cp->get_parent();
		}
	}

public:
	std::vector<CPoint> get_path() { return pathList; }
private:
	int *pathTable;
	CPoint *start;
	CPoint *end;
	std::vector<CPoint *> openList; 
	std::vector<CPoint *> closeList;
	std::vector<CPoint> pathList;
};

int main()
{
	int map[3][7] = {
		{1, 0, 0, 0, 1, 0, 1},
		{0, 0, 1, 0, 0, 1, 0},
		{1, 0, 0, 1, 0, 0, 1},
	};
	CPoint start(1.0, 0.0);
	CPoint end(9/2.0, 1.0);
	AStar* astar = new AStar((int *)map, start, end);
	vector<CPoint> v = astar->get_path();
	for(int i = (int)v.size() - 1; i >= 0; --i)
	{
		cout<<v[i]<<endl;
	}
	delete astar;
	return 0;
}
