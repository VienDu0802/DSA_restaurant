#include "main.h"

class FIFO{
public:
    table* head = nullptr,* last = nullptr;
    int Size = 0;
    int length = 0;
public:

    int size();
    void enFIFO(int, string, int );
    table* deFIFO();
    void deFIFO(string, int);
    void Ssort(int);
    void Print(int);
    void rPrint(table*, int);
    void rPrint(int );

    ~FIFO(){
        while(head){
            table * t = head;
            head = head->next;

            delete t;
        }
    }
};

int FIFO::size(){return Size;}

void FIFO::enFIFO(int ID, string name, int age){    
    Size++;
    table * t = new table(ID, name, age, NULL);
    if(Size == 1) head = last = t;
    else{
        last->next = t;
        last = last->next;
    }
}

table* FIFO::deFIFO(){
    Size --;
    table* t;
    if(Size == 0){
        t = head;
        head = last = NULL;
    }
    else{
        t = head;
        head = head->next;
    }
    return t;
}

void FIFO::deFIFO(string name, int age){
    table* t;
    if(Size == 0) return;
    else if(head->name == name && head->age == age) t = FIFO::deFIFO();
    else{
        Size --;
        table* KhachHang = head;
        while(!((KhachHang->next->name == name)&& (KhachHang->next->age == age))) KhachHang = KhachHang->next;
        if(KhachHang->next == last){
            t = last;
            last = KhachHang;
            last->next = NULL;
        }
        else{
            t = KhachHang->next;
            KhachHang->next = KhachHang->next->next;
        }
    }
    delete t;
}  

void FIFO::Ssort(int number){
    if(Size == 0) cout << "Empty\n";
    table * h = this->head,* tablemax = NULL,* idx = NULL;
    while(number && h){
        idx = tablemax = h;
        number --;
        while (idx){
            if (idx->age > tablemax->age) tablemax = idx;
            idx = idx->next;
        }

        cout << tablemax->name << endl;
        swap(tablemax->age, h->age);
        swap(tablemax->name, h->name);
        swap(tablemax->ID, h->ID);

        
        h = h->next;
    }        
}

void FIFO::Print(int number){
    if(Size == length) cout << "Empty\n";
    else{
        table* idx = head;
        for(int i = 0; i < length; i++) idx = idx->next ;  
        
        for(int i = 0; i < number && idx; i++,idx = idx->next ) cout << idx->name << endl;
    }   
}  

void FIFO::rPrint(table* idx, int number){
    if(number && idx){
        this->rPrint(idx->next, number - 1);
        cout << idx->name << endl;
    }
}

void FIFO::rPrint(int number){
    if(Size == 0){
        cout << "Empty\n";
        return;
    }
    table * KhachHang = head;
    for(int i = 0; i < Size - number; i++) KhachHang = KhachHang->next; 
    this->rPrint(KhachHang, number);
}

void findtable(table*& t, int ID = 1){
    for(int i = 0; i < ID ;i++) t = t->next;
    while(!(t->name == "" && t->age == 0)) t = t->next;       
}

void REG(const string& name,const int& age, int ID,restaurant*& r, FIFO& wait, FIFO& FIFO, table* & last, int &TABLESIZE){
    if(ID <= MAXSIZE && ID > 0 && wait.size() != MAXSIZE && age >= 16 && age <= 115){
        if(TABLESIZE < MAXSIZE){  
            table * KhachHang = r->recentTable;
            findtable(KhachHang, ID);
            KhachHang->name = name;
            KhachHang->age = age; 
    
            FIFO.enFIFO(KhachHang->ID, name, age); 
            FIFO.length ++;
            TABLESIZE++;

            last = KhachHang; 
            return;             
        }   
        FIFO.enFIFO(ID, name, age);  
        wait.enFIFO(ID, name, age);  
    }

}

void REGM(const string& name,const int& age, int number,restaurant*& r, FIFO& FIFO, table* & last, int &TABLESIZE){
    if(age >= 16 && age <= 115 && TABLESIZE != MAXSIZE &&  number > 0 && number <= MAXSIZE){
        int idx = -1, length = 0;
        table* KhachHang = r->recentTable->next;
        for(int i = 1; i < MAXSIZE + number + 1; i++, KhachHang = KhachHang->next){
            if(length >= number) idx = i;
            length = (KhachHang->age == 0 && KhachHang->name == "") ? length + 1 : 0;
        }
        
        if(idx == -1) return;
        
        idx -= number;
        KhachHang = r->recentTable;
        for(int i = 0; i < idx; i++) KhachHang = KhachHang->next;
         last = KhachHang;
        FIFO.enFIFO(KhachHang->ID, name, age); 
        FIFO.length++;
        TABLESIZE += number;

        KhachHang->age = age - 200;
        for(int i = 0; i < number; i++){
            KhachHang->name = name;
            KhachHang->age += 200;
            KhachHang = KhachHang->next;                 
        }
    }        

}

void CLE(const int& ID,restaurant*& r, FIFO& wait, FIFO& FIFO, table* & last, int &TABLESIZE){
    if(ID <= 0 || ID > MAXSIZE) return;
    table * KhachHang = r->recentTable;
    for(int i = 0; i < ID ;i++) KhachHang = KhachHang->next;
    if(KhachHang->age <= 0 || KhachHang->age > 120) return;
    
    FIFO.deFIFO(KhachHang->name, KhachHang->age);
    
    FIFO.length -= 1;
    KhachHang->age = 200;
    while(KhachHang->age == 200){
        
        TABLESIZE --;
        KhachHang->age = 0;
        KhachHang->name = "";
        KhachHang = KhachHang->next;    
    }

    while(!(wait.size() == 0 || TABLESIZE == MAXSIZE)){
    
        table * t = wait.deFIFO();
        table * KhachHang = r->recentTable;
        FIFO.length++;
        TABLESIZE++;     
        findtable(KhachHang,t->ID);
        KhachHang->name = t->name;
        KhachHang->age = t->age;
        
        last = KhachHang;  
        delete t; 
    }    
}

void simulate(string filename, restaurant* r)
{
    ifstream ip(filename);
    string str;


    table* last = NULL;
    FIFO wait, FIFO; 
    int TABLESIZE = 0;
    string KeyWord;
    while(getline(ip, str)){
        int postion = str.find(' ');
        KeyWord = str.substr(0,postion);

        if(KeyWord == "REG"){
            string name;
            int age, ID = 1;
            
            int postion = 4, idx = str.find(' ', 4);
            string s1  = str.substr(postion, idx - postion);
            postion = idx + 1;
            idx = str.find(' ', postion);
            if(idx == -1){
                name = s1;
                age = stoi(str.substr(postion));        
            }
            else{
                ID = stoi(s1);  
                name = str.substr(postion, idx - postion);
                age = stoi(str.substr(idx + 1));   
            }
            REG(name, age, ID,r,wait,FIFO, last, TABLESIZE);
        }
        else if(KeyWord == "REGM"){
            int age, number;
            string name;
            int postion = 5;
            int idx = str.find(' ', postion);
            name  = str.substr(postion, idx - postion);
            postion = idx + 1;
            idx = str.find(' ', postion);
            age = stoi(str.substr(postion, idx - postion));
            number = stoi(str.substr(idx + 1));
            REGM(name, age, number,r,FIFO, last, TABLESIZE);
        }
        else if(KeyWord == "CLE")  CLE(stoi(str.substr(4)),r,wait,FIFO, last, TABLESIZE);
        else if(KeyWord == "PS"){
            if(int(str.find(' ')) == -1) FIFO.rPrint(2*MAXSIZE);
            else{
                int number = stoi(str.substr(3));
                if(number > 0 && number <= 2*MAXSIZE) FIFO.rPrint(number);
            }
        }
        else if(KeyWord == "PQ"){
            if(int(str.find(' ')) == -1) FIFO.Print(MAXSIZE);
            else{
                int number = stoi(str.substr(3));
                if(number > 0 && number <= MAXSIZE) FIFO.Print(number);
            }
        }
        else if(KeyWord == "PT"){
            table* KhachHang = last;
            for(int i = 0; i < MAXSIZE; i++, KhachHang = KhachHang->next) 
                if(KhachHang->age > 0 && KhachHang->age <= 115) cout << KhachHang->ID << "-" << KhachHang->name << "\n";
        } 
        else if(KeyWord == "SQ"){
            int number = stoi(str.substr(2));
            if(number > 0 && number <= MAXSIZE) wait.Ssort(number);
        } 
    }
}

