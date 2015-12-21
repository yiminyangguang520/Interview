
/**********************************************************************************
*              1
*             / \
*            2   3
*
*
*  给定一棵二叉树，仅包含0到9这些数字，每一条从根节点到叶节点的路径表示一个数。例如，
*  路径 1->2->3  表示数值123。求出所有路径表示的数值的和。 上述例子中，路径 1->2
*  表示数值12 ，路径 1->3  表示数值13。它们的和是25
*
*
**********************************************************************************/

#include <iostream>
#include <vector>

using namespace std;

struct TreeNode
{
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x)
        : val(x)
        , left(NULL)
        , right(NULL)
    {

    }
};


int sumNumbers(TreeNode* root)
{
    if (root == NULL)
    {
        return 0;
    }

    int sum = 0;
    vector<int> arr;

    dfs(root, arr, sum);

    return sum;
}

int vec2num(vector<int>& vec)
{
    int num = 0;

    for (auto n : vec)
    {
        num = num * 10 + n;
    }

    return num;
}


void dfs(TreeNode* treeNode, vector<int> & v, int& sum)
{
    if (treeNode == NULL)
    {
        return;
    }

    v.push_back(treeNode->val);

    if (treeNode->left == NULL && treeNode->right == NULL)
    {
        sum += vec2num(v);
    }
    else
    {
        if (treeNode->left != NULL)
        {
            dfs(treeNode->left, v, sum);
        }

        if (treeNode->right != NULL)
        {
            dfs(treeNode->right, v, sum);
        }
    }

    v.pop_back();
}
