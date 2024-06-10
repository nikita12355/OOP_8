#pragma once
#include <iostream>
#include <string>

using namespace std;

const int evNothing = 0;
const int evMessage = 100;

const int cmCreate = 1;
const int cmAdd = 2;
const int cmDelete = 3;
const int cmPrint = 4;
const int cmPrintNames = 5;
const int cmQuit = 6;

class TEvent
{
public:
    int What;
    int Command;
};

class Object
{
public:
    virtual ~Object() {}
    Object() {}
    virtual void Show(bool OnlyNames = false) = 0;
    virtual void PutIn() = 0;
    virtual string GetName() = 0;
};

class Person : public Object
{
protected:
    string name;
    int age;
public:
    Person() {}
    Person(string n, int a)
    {
        SetName(n);
        SetAge(a);
    }
    virtual ~Person() {}
    string GetName() override
    {
        return name;
    }
    int GetAge()
    {
        return age;
    }
    void SetName(string n)
    {
        name = n;
    }
    void SetAge(int a)
    {
        age = a;
    }
    void Show(bool OnlyNames = false) override
    {
        if (OnlyNames)
        {
            cout << GetName() << endl;
            return;
        }
        cout << "Имя: " << name << endl << "Возраст: " << age << endl;
    }
    void PutIn() override
    {
        cout << "Введите имя: " << endl;
        getline(cin, name);
        cout << "Введите возраст: " << endl;
        cin >> age;
        cin.ignore();
    }
    Person& operator=(Person& p)
    {
        name = p.name;
        age = p.age;
        return *this;
    }
};

class Employee : public Person
{
protected:
    float wage;
    string post;
public:
    Employee() : Person() {}
    Employee(string n, int a, float w, string p) : Person(n, a)
    {
        SetWage(w);
        SetPost(p);
    }
    virtual ~Employee() {}
    void SetWage(float w)
    {
        wage = w;
    }
    float GetWage()
    {
        return wage;
    }
    void SetPost(string p)
    {
        post = p;
    }
    string GetPost()
    {
        return post;
    }
    void Show(bool OnlyNames = false) override
    {
        Person::Show(OnlyNames);
        if (!OnlyNames)
        {
            cout << "Зарплата: " << wage << endl << "Должность: " << post << endl;
        }
    }
    void PutIn() override
    {
        Person::PutIn();
        cout << "Введите зарплату: " << endl;
        cin >> wage;
        cin.ignore();
        cout << "Введите должность: " << endl;
        getline(cin, post);
    }
    Employee& operator=(Employee& e)
    {
        Person::operator=(e);
        wage = e.wage;
        post = e.post;
        return *this;
    }
};

class Vector
{
protected:
    Object** elements;
    int Length;
    int cur_Length;
public:
    Vector()
    {
        elements = 0;
        Length = 0;
        cur_Length = 0;
    }
    Vector(int L)
    {
        elements = new Object * [L];
        Length = L;
        cur_Length = 0;
    }
    virtual ~Vector()
    {
        if (elements == 0)
        {
            return;
        }
        for (int i = 0; i < cur_Length; i++)
        {
            delete elements[i];
        }
        delete[] elements;
        Length = 0;
        cur_Length = 0;
    }
    void Recreate(int L)
    {
        if (elements != 0)
        {
            for (int i = 0; i < cur_Length; i++)
            {
                delete elements[i];
            }
            delete[] elements;
        }
        Length = L;
        elements = new Object * [L];
        cur_Length = 0;
    }
    bool TryAdd(Object* E)
    {
        if (cur_Length >= Length)
        {
            cout << "Нет места!" << endl;
            return false;
        }
        elements[cur_Length] = E;
        cur_Length++;
        return true;
    }
    bool TryRemove(string N)
    {
        if (cur_Length == 0)
        {
            cout << "Группа пуста!" << endl;
            return false;
        }
        bool Flag = false;
        for (int i = 0; i < cur_Length && !Flag; i++)
        {
            Flag = elements[i]->GetName() == N;
            if (Flag)
            {
                Object* tmp = elements[i];
                for (int j = i; j < cur_Length; j++)
                {
                    elements[j] = elements[j + 1];
                }
                delete tmp;
                cur_Length--;
            }
        }
        if (!Flag)
        {
            cout << "Такого элемента нет!" << endl;
        }
        return Flag;
    }
    void PrintGroup(bool OnlyNames = false)
    {
        if (cur_Length == 0)
        {
            cout << "Группа пуста!" << endl;
            return;
        }
        elements[0]->Show(OnlyNames);
        for (int i = 1; i < cur_Length; i++)
        {
            cout << endl;
            elements[i]->Show(OnlyNames);
        }
    }
};

class Dialog : public Vector
{
protected:
    bool isOver;
public:
    Dialog()
    {
        isOver = false;
    }
    ~Dialog() {};
    void Execute()
    {
        cout << "m - создать группу, + - добавить элемент, - - удалить элемент, s - вывести список элементов, z - вывести список имён, q - завершение работы" << endl;

        TEvent Event;

        while (!isOver)
        {
            GetEvent(Event);
            HandleEvent(Event);
        }
    }
    void GetEvent(TEvent& Event)
    {
        char CommandSymbol;
        cout << "> ";
        cin >> CommandSymbol;

        Event.What = evMessage;

        switch (CommandSymbol)
        {
        case 'm':
        {
            Event.Command = cmCreate;
            break;
        }
        case '+':
        {
            Event.Command = cmAdd;
            break;
        }
        case '-':
        {
            Event.Command = cmDelete;
            break;
        }
        case 's':
        {
            Event.Command = cmPrint;
            break;
        }
        case 'z':
        {
            Event.Command = cmPrintNames;
            break;
        }
        case 'q':
        {
            Event.Command = cmQuit;
            break;
        }
        default:
        {
            Event.What = evNothing;
        }
        }
    }
    void HandleEvent(TEvent& Event)
    {
        if (Event.What == evNothing)
        {
            cout << "Не знаю такой кнопки!" << endl;
            return;
        }
        string Name;
        switch (Event.Command)
        {
        case cmCreate:
        {
            int Size;
            do
            {
                cout << "Введите размер группы: " << endl;
                cin >> Size;
            } while (Size <= 0);

            Recreate(Size);
            cout << "Готово. Введите следующую команду" << endl;
            break;
        }
        case cmAdd:
        {
            if (elements == 0)
            {
                cout << "Группа не создана!" << endl;
                break;
            }

            int Number;

            do
            {
                cout << "1 - Создать человека, 2 - Создать сотрудника" << endl;
                cin >> Number;
            } while (Number != 1 && Number != 2);

            cin.ignore();

            if (Number == 1)
            {
                Person* Prs = new Person;
                Prs->PutIn();
                TryAdd(Prs);
            }
            else
            {
                Employee* Emp = new Employee;
                Emp->PutIn();
                TryAdd(Emp);
            }
            cout << "Готово. Введите следующую команду" << endl;
            break;
        }
        case cmDelete:
        {
            if (elements == 0)
            {
                cout << "Группа не создана!" << endl;
                break;
            }
            cin.ignore();
            cout << "Введите имя: " << endl;
            getline(cin, Name);
            TryRemove(Name);
            cout << "Готово. Введите следующую команду" << endl;
            break;
        }
        case cmPrint:
        {
            PrintGroup();
            cout << endl;
            cout << "Готово. Введите следующую команду" << endl;
            break;
        }
        case cmPrintNames:
        {
            PrintGroup(true);
            cout << endl;
            cout << "Готово. Введите следующую команду" << endl;
            break;
        }
        case cmQuit:
        {
            isOver = true;
            break;
        }
        default:
        {
            break;
        }
        }
        Event.What = evNothing;
    }
};
