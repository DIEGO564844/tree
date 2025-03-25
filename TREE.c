#include <stdio.h>
#include <stdlib.h>

#define max 100

// 二叉树节点结构体
typedef struct BiTNode {
    int data;  // 节点存储的数据
    struct BiTNode* lchild;  // 左子节点指针
    struct BiTNode* rchild;  // 右子节点指针
    int index;  // 节点索引，可能用于特定标记
    int lhigh;  // 左子树高度
    int rhigh;  // 右子树高度
    int key;  // 平衡因子，左子树高度减去右子树高度
    int lsonindex;  // 左子节点索引标记
    int rsonindex;  // 右子节点索引标记
} BiTNode, * BiTree;

// 显示最小异常子树根节点和其前驱节点信息
void show(BiTNode* finanode, BiTNode* prefinanode);
// 调整二叉树结构以恢复平衡
BiTNode* Astruct(BiTNode* finanode, BiTNode* prefinanode, BiTNode* root);
// 检查并处理二叉树的不平衡情况
//void maxshow(BiTNode* root);

// 创建一个未链接节点
BiTree CreateBiTNode(int data) {
    BiTree tempNode = (BiTree)malloc(sizeof(BiTNode));  // 动态分配节点内存
    if (tempNode == NULL) {
        printf("内存不够，申请空间失败！\n");
        exit(1);  // 内存分配失败，退出程序
    }
    tempNode->data = data;  // 初始化节点数据
    tempNode->lchild = NULL;  // 初始化左子节点指针
    tempNode->rchild = NULL;  // 初始化右子节点指针
    tempNode->lhigh = 0;  // 初始化左子树高度
    tempNode->rhigh = 0;  // 初始化右子树高度
    tempNode->key = 0;  // 初始化平衡因子
    tempNode->index = 0;  // 初始化节点索引
    int lsonindex = 0;  // 初始化左子节点索引标记
    int rsonindex = 0;  // 初始化右子节点索引标记
    return tempNode;
}

// 按一定顺序插入节点
void InsertBiTNode(BiTree* root, int data) {
    if (*root == NULL) {
        *root = CreateBiTNode(data);  // 若树为空，创建新节点作为根节点
        return;
    }
    else if (data < (*root)->data) {
        InsertBiTNode(&((*root)->lchild), data);  // 若数据小于当前节点数据，插入到左子树
    }
    else if (data > (*root)->data) {
        InsertBiTNode(&((*root)->rchild), data);  // 若数据大于当前节点数据，插入到右子树
    }
    else {
        printf("%d已存在, 无需插入.\n", data);  // 数据已存在，给出提示
        return;
    }
}

// 找最小节点
BiTree FindMinNode(BiTree root) {
    if (root == NULL) {
        return NULL;  // 若树为空，返回空指针
    }
    BiTree p = root;
    while (p->lchild != NULL) {
        p = p->lchild;  // 不断向左子树查找，直到找到最小节点
    }
    return p;
}

// 按值删除节点
BiTree DeleteBiTNode(BiTree root, int data) {
    if (root == NULL) {
        return NULL;  // 若树为空，返回空指针
    }
    if (data == root->data) {
        // case1:删除叶子节点
        if (root->lchild == NULL && root->rchild == NULL) {
            free(root);  // 释放节点内存
            return NULL;
        }
        // case2:删除节点有一个孩子节点
        else if (root->lchild == NULL || root->rchild == NULL) {
            BiTree tempChild = (root->lchild != NULL) ? root->lchild : root->rchild;  // 获取子节点
            free(root);  // 释放当前节点内存
            return tempChild;
        }
        // case3:删除节点有两个孩子节点
        else {
            BiTree successor = FindMinNode(root->rchild);  // 找到右子树中的最小节点
            root->data = successor->data;  // 用最小节点的数据替换当前节点数据
            root->rchild = DeleteBiTNode(root->rchild, successor->data);  // 递归删除右子树中的最小节点
            return root;
        }
    }
    else if (data < root->data) {
        root->lchild = DeleteBiTNode(root->lchild, data);  // 若数据小于当前节点数据，在左子树中删除
    }
    else if (data > root->data) {
        root->rchild = DeleteBiTNode(root->rchild, data);  // 若数据大于当前节点数据，在右子树中删除
    }
    return root;
}

// 中序遍历二叉树
void InorderTraversal(BiTree root) {
    if (root != NULL) {
        InorderTraversal(root->lchild);  // 递归遍历左子树
        printf("%d ", root->data);  // 输出当前节点数据
        InorderTraversal(root->rchild);  // 递归遍历右子树
    }
}

// 计算树的高度
int High(BiTree node) {
    if (node == NULL) {
        return 0;  // 若节点为空，高度为0
    }
    int top = 0;  // 队列头指针
    int tail = 0;  // 队列尾指针
    BiTree* num = (BiTree*)malloc(max * sizeof(BiTree));  // 动态分配队列内存
    if (!num) {
        perror("队列内存分配失败");
        exit(-1);  // 内存分配失败，退出程序
    }
    BiTree temp = node;
    num[tail++] = temp;  // 将根节点入队
    int currentLevel = 0;  // 记录当前层次
    while (top < tail) {
        int len = tail - top;  // 当前层的节点数量
        // 遍历当前层的所有节点
        for (int i = 0; i < len; i++) {
            BiTree current = num[top++];  // 出队一个节点
            if (current->lchild) {
                num[tail++] = current->lchild;  // 若有左子节点，入队
            }
            if (current->rchild) {
                num[tail++] = current->rchild;  // 若有右子节点，入队
            }
        }
        currentLevel++;  // 处理完一层，层次加1
    }
    free(num);  // 释放队列内存
    return currentLevel;  // 返回树的高度
}

// 计算每个节点的左右子树高度
BiTree High1(BiTree node) {
    int top = 0;  // 队列头指针
    int tail = 0;  // 队列尾指针
    BiTree* num = (BiTree*)malloc(max * sizeof(BiTree));  // 动态分配队列内存
    if (!num) {
        perror("队列内存分配失败");
        exit(-1);  // 内存分配失败，退出程序
    }
    BiTree temp = node;
    num[tail++] = temp;  // 将根节点入队
    while (top < tail) {
        temp->lhigh = High(temp->lchild);  // 计算左子树高度
        temp->rhigh = High(temp->rchild);  // 计算右子树高度
        temp->key = temp->lhigh - temp->rhigh;  // 计算平衡因子
        printf("数据%d的左子树高度为:%d,右子树高度为:%d,平衡因子为:%d\n", temp->data, temp->lhigh, temp->rhigh, temp->key);
        if (temp->lchild) {
            num[tail++] = temp->lchild;  // 若有左子节点，入队
        }
        if (temp->rchild) {
            num[tail++] = temp->rchild;  // 若有右子节点，入队
        }
        temp = num[++top];  // 处理下一个节点
    }
    free(num);  // 释放队列内存
    return node;
}

// 查找最小异常子树根节点并调整树结构
BiTNode* SM(BiTNode* root) {
    BiTNode* finanode = NULL;  // 最小异常子树根节点
    BiTNode* prefinanode = NULL;  // 最小异常子树根节点的前驱节点
    BiTNode* temp = root;
    int top = -1;
    BiTNode** num = (BiTNode**)malloc(max * sizeof(BiTNode*));  // 动态分配栈内存
    if (!num) {
        perror("内存分配失败");
        exit(-1);  // 内存分配失败，退出程序
    }
    while (temp != NULL || top != -1) {
        while (temp != NULL) {
            num[++top] = temp;  // 节点入栈
            if (temp->key >= 2 || temp->key <= -2) {  // 发现平衡因子异常的节点
                int top8 = 0;
                int tail = 0;
                BiTree** num8 = (BiTree**)malloc(max * sizeof(BiTree*));  // 动态分配队列内存
                if (!num8) {
                    perror("队列内存分配失败");
                    exit(-1);  // 内存分配失败，退出程序
                }
                num8[tail++] = temp;  // 将异常节点入队
                int currentLevel = 0;  // 记录当前层次
                while (top8 < tail) {
                    int len = tail - top8;
                    int level = 0;
                    // 遍历当前层的所有节点
                    BiTree current = NULL;
                    BiTNode* index11 = num8[top8];
                    index11->index++;  // 增加节点索引
                    for (int i = 0; i < len; i++) {
                        current = num8[top8++];  // 出队一个节点
                        if (current && current->lchild) {
                            num8[tail++] = current->lchild;  // 若有左子节点，入队
                        }
                        if (current && current->rchild) {
                            num8[tail++] = current->rchild;  // 若有右子节点，入队
                        }
                        if (current) {
                            current->index++;  // 增加节点索引
                        }
                    }
                }
                int front = -1;
                BiTNode* index2 = temp;
                int count = 0;
                int i = 0;
                BiTNode** num3 = (BiTNode**)malloc(max * sizeof(BiTNode*));  // 动态分配数组内存
                if (!num3) {
                    perror("内存分配失败");
                    exit(0);  // 内存分配失败，退出程序
                }
                BiTNode** num1 = (BiTNode**)malloc(max * sizeof(BiTNode*));  // 动态分配数组内存
                if (!num1) {
                    perror("内存分配失败");
                    exit(-1);  // 内存分配失败，退出程序
                }
                while (index2 != NULL || front != -1) {
                    while (index2 != NULL) {
                        if (index2->key >= 2 || index2->key <= -2) {
                            num3[i++] = index2;  // 记录异常节点
                            count++;
                        }
                        num1[++front] = index2;  // 节点入栈
                        index2 = index2->lchild;  // 向左子树遍历
                    }
                    if (front != -1) {
                        index2 = num1[front--];  // 节点出栈
                        index2 = index2->rchild;  // 向右子树遍历
                    }
                }
                for (int i = 0; i < count; i++) {
                    for (int j = 0; j < count - i - 1; j++) {
                        if (num3[j]->index < num3[j + 1]->index) {
                            BiTNode* index4 = num3[j];
                            num3[j] = num3[j + 1];
                            num3[j + 1] = index4;  // 交换节点顺序
                        }
                    }
                }
                finanode = num3[0];  // 找到最小异常子树根节点
                BiTNode** num4 = (BiTNode**)malloc(max * sizeof(BiTNode*));  // 动态分配数组内存
                if (!num4) {
                    perror("内存分配失败");
                    exit(-1);  // 内存分配失败，退出程序
                }
                int top1 = -1;
                BiTNode* index5 = root;

                while (index5 != NULL || top1 != -1) {
                    while (index5 != NULL) {
                        num4[++top1] = index5;  // 节点入栈
                        index5 = index5->lchild;  // 向左子树遍历
                    }
                    if (top1 != -1) {
                        index5 = num4[top1--];  // 节点出栈
                        if (index5->rchild == finanode || index5->lchild == finanode) {
                            if (index5->rchild == finanode) {
                                prefinanode = index5;  // 记录前驱节点
                                finanode->rsonindex = 1;  // 标记右子节点
                            }
                            if (index5->lchild == finanode) {
                                prefinanode = index5;  // 记录前驱节点
                                finanode->lsonindex = 1;  // 标记左子节点
                            }
                            break;
                        }
                        else {
                            prefinanode = NULL;
                        }
                        index5 = index5->rchild;  // 向右子树遍历
                    }
                }

                int top9 = -1;
                BiTNode* temp12 = temp;
                BiTNode** num78 = (BiTNode**)malloc(max * sizeof(BiTNode*));  // 动态分配数组内存
                if (!num78) {
                    perror("内存分配失败");
                    exit(-1);  // 内存分配失败，退出程序
                }

                while (temp12 != NULL || top9 != -1) {
                    while (temp12 != NULL) {
                        num78[++top9] = temp12;  // 节点入栈
                        temp->index = 0;  // 重置节点索引
                        temp12 = temp12->lchild;  // 向左子树遍历
                    }
                    if (top != -1) {
                        temp12 = num78[top9--];  // 节点出栈
                        temp12 = temp12->rchild;  // 向右子树遍历
                    }
                }
                free(num1);  // 释放数组内存
                free(num3);  // 释放数组内存
                free(num4);  // 释放数组内存
                free(num8);  // 释放队列内存
            }
            temp = temp->lchild;  // 向左子树遍历
        }
        if (top != -1) {
            temp = num[top--];  // 节点出栈
            temp = temp->rchild;  // 向右子树遍历
        }

    }
    show(finanode, prefinanode);  // 显示最小异常子树根节点和其前驱节点信息
    root = Astruct(finanode, prefinanode, root);  // 调整树结构
    printf("结构已修饰\n");
    printf("修饰后的中序遍历结果: ");
    InorderTraversal(root);  // 输出修饰后的中序遍历结果
    free(num);  // 释放栈内存
    return root;
}

// 调整二叉树结构以恢复平衡
BiTNode* Astruct(BiTNode* finanode, BiTNode* prefinanode, BiTNode* root) {
    BiTNode* newRoot = NULL;  // 新的根节点
    BiTNode* temp = finanode;
    BiTNode* pretemp = prefinanode;
    if (pretemp) {
        if (temp->lhigh == 0 && ((temp->rchild->lchild == NULL && temp->rchild->rchild) || (temp->rchild->lchild && temp->rchild->rchild == NULL))) {
            temp->rchild->lchild = temp;  // 调整节点连接关系
            if (temp->lsonindex == 1) {
                pretemp->lchild = temp->rchild;  // 调整父节点的左子节点
            }
            else {
                pretemp->rchild = temp->rchild;  // 调整父节点的右子节点
            }
            temp->lchild = temp->rchild = NULL;  // 清空当前节点的左右子节点
        }
        if (temp->rhigh == 0 && ((temp->lchild->rchild == NULL && temp->lchild->lchild) || (temp->lchild->rchild && temp->lchild->lchild == NULL))) {
            temp->lchild->rchild = temp;  // 调整节点连接关系
            if (temp->lsonindex == 1) {
                pretemp->lchild = temp->lchild;  // 调整父节点的左子节点
            }
            else {
                pretemp->rchild = temp->lchild;  // 调整父节点的右子节点
            }
            temp->lchild = temp->rchild = NULL;  // 清空当前节点的左右子节点
        }
        if (temp->rchild && temp->lchild) {
            if (temp->rhigh > temp->lhigh) {
                temp->rchild->lchild = temp;  // 调整节点连接关系
                if (temp->lsonindex == 1) {
                    pretemp->lchild = temp->rchild;  // 调整父节点的左子节点
                }
                else {
                    pretemp->rchild = temp->rchild;  // 调整父节点的右子节点
                }
                temp->rchild = NULL;  // 清空当前节点的右子节点
            }
            if (temp->lhigh > temp->rhigh) {
                temp->lchild->rchild = temp;  // 调整节点连接关系
                if (temp->lsonindex == 1) {
                    pretemp->lchild = temp->rchild;  // 调整父节点的左子节点
                }
                else {
                    pretemp->rchild = temp->rchild;  // 调整父节点的右子节点
                }
                temp->lchild = NULL;  // 清空当前节点的左子节点
            }
        }
        if (temp->lchild == NULL && temp->rchild) {
            BiTNode* index = temp->rchild;
            if (index->lchild && index->rchild) {
                index->lchild->lchild = temp;  // 调整节点连接关系
                if (temp->lsonindex == 1) {
                    pretemp->lchild = temp->rchild;  // 调整父节点的左子节点
                }
                else {
                    pretemp->rchild = temp->rchild;  // 调整父节点的右子节点
                }
                temp->rchild = NULL;  // 清空当前节点的右子节点
            }
        }
        if (temp->rchild == NULL && temp->lchild) {
            BiTNode* index = temp->lchild;
            if (index->lchild && index->rchild) {
                index->rchild->rchild = temp;  // 调整节点连接关系
                if (temp->lsonindex == 1) {
                    pretemp->lchild = temp->rchild;  // 调整父节点的左子节点
                }
                else {
                    pretemp->rchild = temp->rchild;  // 调整父节点的右子节点
                }
                temp->lchild = NULL;  // 清空当前节点的左子节点
            }
        }
        return root;
    }
    else { // pretemp 为空，即 finanode 是根节点
        if (temp->lchild == NULL && temp->rchild) {
            BiTNode* index = temp->rchild;
            if (index->lchild && index->rchild) {
                index->lchild->lchild = temp;  // 调整节点连接关系
                newRoot = index;  // 更新根节点
                temp->rchild = NULL;  // 清空当前节点的右子节点
            }

            return newRoot;
        }
        if (temp->rchild == NULL && temp->lchild) {
            BiTNode* index = temp->lchild;
            if (index->lchild && index->rchild) {
                index->rchild->rchild = temp;  // 调整节点连接关系
                newRoot = index;  // 更新根节点
                temp->lchild = NULL;  // 清空当前节点的左子节点
            }
            return newRoot;
        }
        if (temp->lhigh == 0) {
            BiTNode* rightChild = temp->rchild;
            if (rightChild) {
                rightChild->lchild = temp;  // 调整节点连接关系
            }
            temp->lchild = temp->rchild = NULL;  // 清空当前节点的左右子节点
            newRoot = rightChild;  // 更新根节点
        }
        else if (temp->rhigh == 0) {
            BiTNode* leftChild = temp->lchild;
            if (leftChild) {
                leftChild->rchild = temp;  // 调整节点连接关系
            }
            temp->lchild = temp->rchild = NULL;  // 清空当前节点的左右子节点
            newRoot = leftChild;  // 更新根节点
        }
        else if (temp->rchild && temp->lchild) {
            if (temp->rhigh > temp->lhigh) {
                BiTNode* rightChild = temp->rchild;
                rightChild->lchild = temp;  // 调整节点连接关系
                temp->rchild = NULL;  // 清空当前节点的右子节点
                newRoot = rightChild;  // 更新根节点
            }
            else if (temp->lhigh > temp->rhigh) {
                BiTNode* leftChild = temp->lchild;
                leftChild->rchild = temp;  // 调整节点连接关系
                temp->lchild = NULL;  // 清空当前节点的左子节点
                newRoot = leftChild;  // 更新根节点
            }
        }

        return newRoot;
    }

}

// 显示最小异常子树根节点和其前驱节点信息
void show(BiTNode* finanode, BiTNode* prefinanode) {
    printf("finanode:%d\n", finanode->data);  // 输出最小异常子树根节点的数据
    if (prefinanode == NULL) {
        printf("prefinanode:NULL\n");  // 若前驱节点为空，输出提示信息
    }
    else {
        printf("prefinanode:%d", prefinanode->data);  // 输出前驱节点的数据
    }
}

// 检查并处理二叉树的不平衡情况
void maxshow(BiTNode* root) {
    BiTNode* temp = root;
    int top = -1;
    BiTNode** num = (BiTNode**)malloc(max * sizeof(BiTNode*));  // 动态分配栈内存
    if (!num) {
        perror("内存分配失败");
        exit(-1);  // 内存分配失败，退出程序
    }
    int hasAbnormal = 0;
    while (temp != NULL || top != -1) {
        while (temp != NULL) {
            num[++top] = temp;  // 节点入栈
            if (temp->key >= 2 || temp->key <= -2) {
                hasAbnormal = 1;  // 标记存在异常节点
            }
            temp = temp->lchild;  // 向左子树遍历
        }
        if (top != -1) {
            temp = num[top--];  // 节点出栈
            temp = temp->rchild;  // 向右子树遍历
        }
    }
    if (hasAbnormal) {
        printf("最小异常子树根节点和前驱节点为:\n");
        root = SM(root);  // 处理不平衡情况
    }
    else {
        printf("无异常因子\n");  // 无异常节点，输出提示信息
    }
    free(num);  // 释放栈内存
    return;
}

int main() {
    BiTree root = NULL;
    int choice, data;
    while (1) {
        printf("请选择操作：1.删除  2.插入  3.计算节点子树高度  4.跟踪函数 5.退出程序\n");
        (void)scanf("%d", &choice);  // 读取用户输入的操作选择

        switch (choice) {
        case 1:
            printf("请输入要删除的数据：");
            (void)scanf("%d", &data);  // 读取要删除的数据
            root = DeleteBiTNode(root, data);  // 调用删除节点函数
            printf("删除操作后的中序遍历结果: ");
            InorderTraversal(root);  // 输出删除操作后的中序遍历结果
            printf("\n");
            root = High1(root);  // 计算节点子树高度和平衡因子
            printf("\n");
            maxshow(root);  // 检查并处理不平衡情况
            printf("\n");
            break;
        case 2:
            printf("请输入要插入的数据：");
            (void)scanf("%d", &data);  // 读取要插入的数据
            InsertBiTNode(&root, data);  // 调用插入节点函数
            printf("插入操作后的中序遍历结果: ");
            InorderTraversal(root);  // 输出插入操作后的中序遍历结果
            printf("\n");
            root = High1(root);  // 计算节点子树高度和平衡因子
            printf("\n");
            maxshow(root);  // 检查并处理不平衡情况
            printf("\n");
            break;
        case 3:
            root = High1(root);  // 计算节点子树高度和平衡因子
            break;
        case 4: {
            root = High1(root);  // 计算节点子树高度和平衡因子
            maxshow(root);  // 检查并处理不平衡情况
            printf("\n");
            break;
        }
        case 5:
            exit(0);  // 退出程序
        default:
            printf("无效操作，请重新输入。\n");  // 输入无效操作，给出提示
            break;
        }
    }

    return 0;
}