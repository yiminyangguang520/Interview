#include <iostream>
#include <vector>

using namespace std;

/********************************************************************************** 
* 
* 查找m x n的矩阵中的元素. 
* 该矩阵具有以下特性:
* 
* 每一行从左到右升序排列.
* 每一行的第一个元素大于上一行的最后一个元素.
* 
* 
* 
* 例如，如下矩阵:
* 
* [
*   [1,   3,  5,  7],
*   [10, 11, 16, 20],
*   [23, 30, 34, 50]
* ]
* 
* 给定元素3, 返回true.
*               
**********************************************************************************/

bool search_element(vector<vector<int> > &v, int target)
{
    int row = v.size();
    int col = row > 0 ? v[0].size() : 0;

    int low = 0;
    int length = row * col;
    int high = length - 1;
    int mid = low + (high - low)/2;

    while (low <= high)
     {
      int r = mid / col;
      int c = mid % col;

      if (target == v[r][c]) 
      {
        return true;
      }
      else if (target > v[r][c]) 
      {
        low = mid + 1;
      }
      else
      {
        high = mid - 1;
      }
    }
}
