#include <iostream>
#include <initializer_list>
#define tab "\t"
using namespace std;

class Tree{
    class Element{
        int Data;
        Element *pLeft, *pRight;
    public:
        Element(int Data, Element* pLeft = nullptr, Element* pRight = nullptr): Data(Data), pLeft(pLeft), pRight(pRight){
            //cout  << "EConstructor\t" << this << endl;
        }
        ~Element(){
            //cout  << "EDestructor\t" << this << endl;
        }
        bool is_leaf(){
            return  pLeft == pRight;
        };
        friend class Tree;
    }*Root;
    void print(Element* Root){
        if(Root == nullptr)return;
        print(Root->pLeft);
        cout << Root->Data << "\t";
        print(Root->pRight);
    }
    void insert(int Data, Element* Root){ // Корневой элемент от ветки
        if (this->Root == nullptr)this->Root = new Element(Data);
        if (Root == nullptr) //Если зашли в ветку а она пустая - выходим из нее
            return;
        if(Data < Root->Data){
            if(Root->pLeft == nullptr)
                Root->pLeft = new Element(Data);
            else insert(Data, Root->pLeft);
        }else{
            if(Root->pRight)insert(Data, Root->pRight);
            else Root->pRight = new Element(Data);
        }
    }
    void erase(int Data, Element*& Root){
       if (Root == nullptr)return;
        erase(Data, Root->pLeft);
        erase(Data, Root->pRight);
       if (Data == Root->Data){
           if(Root->is_leaf()){
               delete Root;
               Root = nullptr;
           }else{
               if (Root->pLeft){
                   Root->Data = max(Root->pLeft);
                   erase(max(Root->pLeft), Root->pLeft);
               }else{
                   Root->Data = min(Root->pRight);
                   erase(min(Root->pRight), Root->pRight);
               }
           }
       }
    }
    void clear(Element* Root){
        if(Root == nullptr) return;
        clear(Root->pLeft);
        clear(Root->pRight);
        delete Root;
    }
    int min(Element* Root){
        if (Root->pLeft == nullptr)
            return Root->Data;
        else return min(Root->pLeft);
    }
    int max(Element* Root){
        return Root->pRight ? max(Root->pRight) : Root->Data;
    }
    int count(Element* Root){
        if(Root == nullptr)return 0;
        return (Root->is_leaf()) ? 1 : count(Root->pLeft) + count(Root->pRight) + 1;
    }
    int summ(Element* Root){
        return Root == nullptr ? 0 : summ(Root->pLeft) + summ(Root->pRight) + Root->Data;
    }
public:
    class Iterator{
        Element*& Root;
        Element* Temp;
    public:
        Iterator(Element* Root) : Temp(Temp){
            cout << "ITConstructor:\t" << this << endl;
        }
        ~Iterator(){
            cout << "ITDestructor:\t" << this << endl;
        }
        int& operator*(){
            return Temp->Data;
        }
        const int& operator*()const{
            return Temp->Data;
        }
        bool operator==(const Iterator& other)const{
            return this->Temp == other.Temp;
        }
        bool operator!=(const Iterator& other)const{
            return this->Temp != other.Temp;
        }
        Iterator& operator++(){
            find_next();
            return *this;
        }/*
        operator Element*()const{
            return Temp;
        }*/
        Element*& operator->(){
            return Temp;
        }
    };
    Iterator begin(){
        return find(min(this->Root),this->Root);
    }
    Iterator end(){
        return  find(max(this->Root), this->Root);
    }
    Element*& find(int Data, Element*& Root){
        if (Root == nullptr)return Root;
        find(Data, Root->pLeft);
        find(Data, Root->pRight);
        if (Root->Data == Data)
            return Root;
    }
    Element*& find_next(Element *& Root, Element* Parent = nullptr){
        if(Root->pLeft == nullptr)return Root;
        if(Root->pRight)find_next(Root->pRight, Root);
        find_next(Root->pLeft, Root);
    }
    Element*& find_next(){
        find_next(this->Root);
    }
    Element* getRoot(){
        return this->Root;
    }
    Tree(): Root(nullptr){
        //cout  << "TConstructor\t" << this << endl;
    }
    Tree(const Tree &other) : Tree(){
        copy(other.Root);
    }
    Tree(const initializer_list<int> il):Tree(){
        for(int i:il){
            this->insert(i);
        }
    }
    ~Tree(){
        clear(this->Root);
        //cout  << "TDestructor\t" << this << endl;
    }
    void copy(Element* Root){
        if (Root == nullptr)
            return;
        if(Root) insert(Root->Data);
        if (Root->pLeft) copy(Root->pLeft);
        if (Root->pRight) copy(Root->pRight);
        return;
    }
    void insert(int Data){
        insert(Data, this->Root);
    }
    void print(){
        print(this->Root);
        if (this->Root != nullptr)
            cout << "Root: " << this->Root->Data << endl;
    }
    void clear(){
        clear(this->Root);
        this->Root = nullptr;
    }
    int min(){
        if (this->Root == nullptr)
            return 0;
        return min(this->Root);
    }
    int max(){
        return this->Root ? max(this->Root) : 0;
    }
    int size(){
        return this->Root ? count(this->Root) : 0;
    }
    int summ(){
        return summ(this->Root);
    }
    double avg(){
        return double (summ(this->Root)) / count(this->Root);
    }
    void erase(int Data){
        erase(Data, Root);
    }
    Tree operator=(const Tree &other){
        this->clear();
        this->copy(other.Root);
        cout << "Copy Assigment/t" << this << endl;
        return *this;
    }
};
int main() {
    setlocale(LC_ALL, "");
    srand(time(0));
    Tree T1;
    int n;
    cout << "Enter Tree size: "; cin >> n;
    for(int i = 0; i < n; i++){
        T1.insert(rand()%100);
    }
    T1.print();
    cout << "Минимальное значение дерева\t" << T1.min() << endl;
    cout << "Максимальное значение дерева\t" << T1.max() << endl;
    cout << "Количество элементов дерева\t" << T1.size() << endl;
    cout <<  "Сумма элементов дерева\t" << T1.summ() << endl;
    cout <<  "Среднее арифметическое элементов дерева\t" << T1.avg() << endl;
    for(Tree::Iterator it = T1.begin(); it != T1.end(); ++it){
        cout << *it << tab;
    }
    cout << endl;
     /*
    Tree<int> T3 = {50, 25, 16 ,32, 8, 85, 16, 62, 62, 80, 91, 98};
    T3.print();
    T3.erase(8);
    T3.print();*/
    return 0;
}
