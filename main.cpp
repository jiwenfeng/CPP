// A星算法类 实现A星算法的逻辑
//////////////////////////////////////////////////////////////////////////////////////////
//1. 从起点A开始, 把它作为待处理的方格存入一个"开启列表", 开启列表就是一个等待检查方格的列表.
//2. 寻找起点A周围可以到达的方格, 将它们放入"开启列表", 并设置它们的"父方格"为A.
//3. 从"开启列表"中删除起点 A, 并将起点 A 加入"关闭列表", "关闭列表"中存放的都是不需要再次检查的方格
//4. 检查它所有相邻并且可以到达 (障碍物和 "关闭列表" 的方格都不考虑) 的方格. 如果这些方格还不在 "开启列表" 里的话, 将它们加入 "开启列表",
//   计算这些方格的 G, H 和 F 值各是多少, 并设置它们的 "父方格" 为 C.
//5. 如果某个相邻方格 D 已经在 "开启列表" 里了, 检查如果用新的路径 (就是经过C 的路径) 到达它的话, G值是否会更低一些, 如果新的G值更低,
//   那就把它的 "父方格" 改为目前选中的方格 C, 然后重新计算它的 F 值和 G 值 (H 值不需要重新计算, 因为对于每个方块, H 值是不变的). 
//   如果新的 G 值比较高, 就说明经过 C 再到达 D 不是一个明智的选择, 因为它需要更远的路, 这时我们什么也不做.
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <algorithm>

using namespace std;

// 坐标是否能走 0，能 1，不能
enum CAN_GO{ YES = 0, NO = 1 };


// Point类  表示一个二维坐标
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

	//获得曼哈顿距离
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

	// 获得欧几里得距离
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
			// 不是障碍 能走
			if(*(pathTable+row*3+col) == YES )
			{
				v.push_back(CPoint(x, y));
			}
		}
	}

	//路径规划，算法的核心
	void pathPlanning()
	{
		// 每个路径方向移动坐标修改值
	//	float dir[][2] = {{1, 0}, {1/2, 1}, {-1/2, 1}, {-1, 0}, {-1/2, -1}, {1/2, -1}};
		// 起点加入open列表
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
				// 在closeList中
				if(find_if(closeList.begin(), closeList.end(), cmp(&*it)) != closeList.end())
				{
					continue;
				}
				// 在openList中
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
				else // 不在openList中
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
		std::cout<<"没有路可走了"<<std::endl;
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
