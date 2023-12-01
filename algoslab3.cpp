#include <iostream>
#include <fstream>
#include <queue>
#include <stack>
#include <vector>
#include <string>

using namespace std;

// Структура для представления узла двоичного дерева
struct Node 
{
    int data;
    Node* left;
    Node* right;
 
};
struct Trunk
{
    Trunk* prev;
    string str;
    Trunk(Trunk* prev, string str) {
        this->prev = prev;
        this->str = str;
    }
};
struct AVLTree {

    struct TreeNode {
        int data;
        int height;
        TreeNode* left;
        TreeNode* right;

        TreeNode(int value)
            : data(value), height(1), left(nullptr), right(nullptr) {}
    };

    TreeNode* root;



public:
    AVLTree() : root(nullptr) {}

    void insert(int value);
    TreeNode* insertAVL(TreeNode* root, int value) {
        if (root == nullptr) {
            root = new TreeNode(value);
            return root;
        }

        if (value < root->data) {
            root->left = insertAVL(root->left, value);
        }
        else if (value > root->data) {
            root->right = insertAVL(root->right, value);
        }

        // Обновление высоты узла
        root->height = 1 + max(getHeight(root->left), getHeight(root->right));

        // Проверка нарушения баланса
        int balance = getBalance(root);

        // Малое правое вращение (RR)
        if (balance < -1 && value > root->right->data) {
            return rotateLeft(root);
        }

        // Малое левое вращение (LL)
        if (balance > 1 && value < root->left->data) {
            return rotateRight(root);
        }

        // Большое правое вращение (RL)
        if (balance < -1 && value < root->right->data) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        // Большое левое вращение (LR)
        if (balance > 1 && value > root->left->data) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        return root;
    }

    // Получение высоты поддерева
    int getHeight(TreeNode* node) {
        if (node == nullptr) {
            return 0;
        }

        return node->height;
    }

    // Получение баланса узла
    int getBalance(TreeNode* node) {
        if (node == nullptr) {
            return 0;
        }

        return getHeight(node->left) - getHeight(node->right);
    }

    // Вращение влево
    TreeNode* rotateLeft(TreeNode* root) {
        TreeNode* newRoot = root->right;
        TreeNode* subtree = newRoot->left;

        newRoot->left = root;
        root->right = subtree;

        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

        return newRoot;
    }

    // Вращение вправо
    TreeNode* rotateRight(TreeNode* root) {
        TreeNode* newRoot = root->left;
        TreeNode* subtree = newRoot->right;

        newRoot->right = root;
        root->left = subtree;

        root->height = 1 + max(getHeight(root->left), getHeight(root->right));
        newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));

        return newRoot;
    }
};
// Создание нового узла дерева
Node* createNode(int data) 
{
    Node* newNode = new Node();
    if (!newNode) {
        cout << "Ошибка при выделении памяти!" << endl;
        return nullptr;
    }
    newNode->data = data;
    newNode->left = newNode->right = nullptr;
    return newNode;
}

// Чтение скобочной записи из файла
void readExpressionFromFile(const string& aboba, string& expression)
{
    ifstream file(aboba);
    if (file.is_open())
    {
        getline(file, expression);
        file.close();
    }
    else {
        cout << "Ошибка при открытии файла!" << endl;
    }
}
void writeBinaryTreeToFile(Node* root, std::ofstream& file) {
    if (root == nullptr) {
        return;
    }

    file << root->data;

    if (root->left != nullptr || root->right != nullptr) {
        file << "(";
        writeBinaryTreeToFile(root->left, file);

        if (root->right != nullptr) {
            file << ",";
            writeBinaryTreeToFile(root->right, file);
        }

        file << ")";
    }
}
// Проверка, является ли символ оператором
bool isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Проверка, является ли символ числом
bool isNumber(char c) {
    return (c >= '0' && c <= '9');
}

// Парсинг числа из строки
int parseNumber(string::iterator& it) {
    int num = 0;
    while (isNumber(*it)) {
        num = num * 10 + (*it - '0');
        it++;
    }
    return num;
}
bool checkParentheses(const std::string& expression) {
    std::stack<char> st;
    for (char c : expression) {
        if (c == '(') {
            st.push(c);
        }
        else if (c == ')') {
            if (st.empty()) {
                return false; // Неправильная скобочная последовательность
            }
            st.pop();
        }
    }
    return st.empty(); // Скобки сбалансированы, если стек пуст
}


// Построение двоичного дерева по скобочной записи
Node* buildBinaryTree(string expression) {
    stack<Node*> st;
    Node* root = nullptr;
    Node* temp = nullptr;

    for (auto it = expression.begin(); it != expression.end(); it++) 
    {
        if (isNumber(*it)) 
        {
            int num = parseNumber(it);
            temp = createNode(num);
            st.push(temp);
        }
        else if (isOperator(*it)) {
            temp = createNode(*it);
            temp->right = st.top();
            st.pop();
            temp->left = st.top();
            st.pop();
            st.push(temp);
        }
    }

    if (!st.empty()) 
    {
        root = st.top();
        st.pop();
    }

    return root;
}

// Очистка памяти, выделенной под дерево
void deleteTree(Node* node) 
{
    if (node == nullptr) 
    {
        return;
    }
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

// Распечатка узлов дерева в ширину
void printBFS(Node* root) {
    if (root == nullptr) {
        return;
    }

    queue<Node*> q;
    q.push(root);

    while (!q.empty()) {
        Node* node = q.front();
        cout << node->data << " ";
        q.pop();

        if (node->left != nullptr) {
            q.push(node->left);
        }
        if (node->right != nullptr) {
            q.push(node->right);
        }
    }
}

// Распечатка узлов дерева в префиксном порядке
void printPreOrder(Node* node) {
    if (node == nullptr) {
        return;
    }

    cout << node->data << " ";
    printPreOrder(node->left);
    printPreOrder(node->right);
}

// Распечатка узлов дерева в инфиксном порядке
void printInOrder(Node* node) {
    if (node == nullptr) {
        return;
    }

    printInOrder(node->left);
    cout << node->data << " ";
    printInOrder(node->right);
}

// Распечатка узлов дерева в постфиксном порядке
void printPostOrder(Node* node) {
    if (node == nullptr) {
        return;
    }

    printPostOrder(node->left);
    printPostOrder(node->right);
    cout << node->data << " ";
}
Node* createBinaryTree(const std::string& expression) {
    std::stack<Node*> st;
    Node* root = nullptr;
    Node* currNode = nullptr;

    for (char c : expression) {
        if (c == '(') {
            st.push(currNode);
        }
        else if (c == ')') {
            st.pop();
        }
        else {
            int data = c - '0';
            currNode = new Node();
            currNode->data = data;
            currNode->left = nullptr;
            currNode->right = nullptr;

            if (root == nullptr) {
                root = currNode;
            }
            else {
                Node* parent = st.top();
                if (parent->left == nullptr) {
                    parent->left = currNode;
                }
                else {
                    parent->right = currNode;
                }
            }
        }
    }

    return root;
}
void showTrunk(Trunk* p, int& count) {//функция вывода связей дерева
    if (p == NULL)//если нет поддеревьев
        return;
    showTrunk(p->prev, count);//выводим предыдущий узел
    count++;//увеличиваем уровень
    cout << p->str;//выводим отступы и связи
}
void print(Node* aBranch, Trunk* prev, bool isRight) {
    if (aBranch == NULL)//пустое дерево
        return;
    string prev_str = "    ";//отступ по уровням (длина как для стрелки)
    Trunk* tmp = new Trunk(prev, prev_str);//новая связь
    print(aBranch->right, tmp, 1);//правое поддерево
    if (!prev)//если нет предыдущего узла (предка) -> корень дерева
        tmp->str = "-->";//связь корня дерева
    else if (isRight) {//если правое поддерево
        tmp->str = ".-->";//связь правого поддерева
        prev_str = "   |";//в отступ по уровням добавляем черту (дерево идет вширь)
    }
    else {//в противном случае - левое поддерево
        tmp->str = "`-->";//левое поддерево
        prev->str = prev_str;//отступ по уровням не меняется
    }
    int count = 0;//уровень узла
    showTrunk(tmp, count);//выводим связи дерева - стебли
    cout << aBranch->data << endl;//выводим значение узла
    if (prev)//задаем строку отступов для узла, если есть поддеревья
        prev->str = prev_str;
    tmp->str = "   |";//в отступ по уровням добавляем черту (дерево идет вширь)
    print(aBranch->left, tmp, 0);//левое поддерево
}
// Вставка нового узла в АВЛ (или КЧ) дерево
int inputIntData()
{
    int N;
    int m = 0, M = 32000;
    for (;;) {
        if ((cin >> N).good() && (m <= N) && (N < M)) return N;
        if (cin.fail()) {
            cin.clear();
            cout << "Неверный ввод, повторите.\n";
            cout << "Введите значение\n";
        }
        else {
            cout << "Число вне допустимого диапазона значений. Повторите ввод.\n";
            cout << "Введите значение\n";
        }
        cin.ignore(100, '\n');
    }
}
void menu() {
    cout << "Меню:" << endl;
    cout << "1) Чтение скобочной записи (путем чтения из файла) и создание двоичного дерева." << endl;
    cout << "2) Обход полученного дерева и создание АВЛ (или КЧ) дерева." << endl;
    cout << "3) Вывод всех узлов дерева: в ширину и 3 в глубину." << endl;
    cout << "4) Выход из программы." << endl;
}

int main() {
    setlocale(LC_ALL, "Rus");
    int choose = 0;
    string expression;
    Node* root = nullptr;
    do {
        menu();
        choose=inputIntData();
        switch (choose) {
        case 1:
            readExpressionFromFile("inputin.txt", expression);

            cout << expression << endl;
            if (!checkParentheses(expression)) {
                cout << "Invalid expression" << std::endl;
                break;
            }
            root = createBinaryTree(expression);
            if (root != nullptr)
            {
                print(root, NULL, 0);
                cout << endl;
                cout << "Двоичное дерево построено успешно!" << endl;
            }
            else 
            {
                cout << "Ошибка при построении дерева!" << endl;
            }

        
            break;
        case 2:
        {
            AVLTree avlTree;
            // Создание АВЛ (или КЧ) дерева
            AVLTree::TreeNode * avlRoot = nullptr;

            // Ввод количества узлов
            int numNodes;
            cout << "Введите количество узлов: ";
            numNodes = inputIntData();

            // Ввод значений узлов
            for (int i = 0; i < numNodes; i++) {
                int value;
                cout << "Введите значение узла " << (i + 1) << ": ";
                value = inputIntData();

                avlRoot = avlTree.insertAVL(avlRoot, value);
            }

            cout << "АВЛ (или КЧ) дерево успешно создано!" << endl;
            }
            break;
        case 3: {
            if (root != nullptr) {
                cout << "Обход в ширину: ";
                printBFS(root);
                cout << endl;
                cout << "Обход в префиксном порядке: ";
                printPreOrder(root);
                cout << endl;
                cout << "Обход в инфиксном порядке: ";
                printInOrder(root);
                cout << endl;
                cout << "Обход в постфиксном порядке: ";
                printPostOrder(root);
                cout << endl;
            }
        }
            break;
        case 4: {
            cout << "До новых встреч :) "<<endl;
        }break;
        default:
            cout << "Неправильно введенный номер, повторите попытку!" << endl;
            break;
        }
        
    } while (choose != 4);
    deleteTree(root);
    return 0;
}