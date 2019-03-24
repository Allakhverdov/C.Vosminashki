//
//С.Восьминашки
//

#include<iostream>
#include<string>
#include<vector>
#include<queue>
#include<stack>
#include<map>
#include<set>
struct Position{                                    //вершина графа
    Position() = default;
    Position(int Square, int digit): Square(Square), digit(digit){}
    int Square = 0;
    void GetNeighbour(std::vector<Position>&Neighbour, std::vector<std::string>&Side);
private:
    int digit = 0;
    int GetL();
    int GetR();
    int GetU();
    int GetD();
    bool operator () (const Position& x, const Position& y) const;
};

const bool operator < (const Position& x, const Position& y)
{
    if(x.Square < y.Square)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Position::operator () (const Position& x, const Position& y) const
{
    return x < y;
}

int Position::GetL()                            //сдвиг нуля влево
{
    int t_digit = 1;
    for(int i = 0 ; i < digit; ++i)
    {
        t_digit *= 10;
    }
    int Left_digit = Square/(t_digit * 10) % 10;
    int Square_ = (Square/(t_digit * 100)*(t_digit * 100) + 8 * t_digit * 10 + Left_digit * t_digit + Square % t_digit) % 1000000000;
    return Square_;
}

int Position::GetR()                            //сдвиг нуля вправо
{
    int t_digit = 1;
    for(int i = 0 ; i < digit - 1; ++i)
    {
        t_digit *= 10;
    }
    int Right_digit = Square/(t_digit) % 10;
    int Square_ = (Square/(t_digit * 100) * t_digit * 100 + Right_digit * t_digit * 10 + 8 * t_digit + Square % t_digit) % 1000000000;
    return Square_;
}

int Position::GetU()                               //сдвиг нуля вверх
{
    int t_digit = 1;
    for(int i = 0 ; i < digit; ++i)
    {
        t_digit *= 10;
    }
    int s1 = Square/(t_digit * 10000) * (t_digit * 10000);
    int s2 = (Square/(t_digit * 1000) % 10) * (t_digit);
    int s3 = (Square % (t_digit * 1000)) / (t_digit * 10) * (t_digit * 10);
    int s4 = Square % t_digit;
    int Square_ = s1 + 8 * t_digit * 1000 + s2 + s3 + s4;
    return Square_;
}

int Position::GetD()                               //сдвиг нуля вниз
{
    int t_digit = 1;
    for(int i = 0 ; i < digit - 3; ++i)
    {
        t_digit *= 10;
    }
    int s1 = Square/(t_digit * 10000) * (t_digit * 10000);
    int s2 = (Square/(t_digit) % 10) * (t_digit * 1000);
    int s3 = (Square % (t_digit * 1000)) / (t_digit * 10) * (t_digit * 10);
    int s4 = Square % t_digit;
    int Square_ = s1 + 8 * t_digit + s2 + s3 + s4;
    return Square_;
}

void Position::GetNeighbour(std::vector<Position>&Neighbour, std::vector<std::string>&Side)     //получаем все соседние позиции
{                                                                                               //запоминаем при этом в какую сторону мы сдвинулись
    if(digit % 3 != 2)
    {
        int LNeighb = GetL();
        Position LN(LNeighb, digit + 1);
        Neighbour.push_back(LN);
        Side.push_back("L");
    }
    if(digit % 3 != 0)
    {
        int LNeighb = GetR();
        Position RN(LNeighb, digit - 1);
        Neighbour.push_back(RN);
        Side.push_back("R");
    }
    if(digit < 6)
    {
        int UNeighb = GetU();
        Position UN(UNeighb, digit + 3);
        Neighbour.push_back(UN);
        Side.push_back("U");
    }
    if(digit > 2)
    {
        int DNeighb = GetD();
        Position DN(DNeighb, digit - 3);
        Neighbour.push_back(DN);
        Side.push_back("D");
    }
}

std::stack<std::string> BFS (Position start)            //сам алгоритм, просто BFS
{
    std::stack<std::string>Sequence;                    //тут будет ответ
    std::queue<Position>Queue;                          //бфс-ная очередь
    std::set<Position>Used;                             //позиции которые уже прошли
    std::map<Position, Position>Father;                 //позиция из которой мы пришли в данную
    std::map<Position, std::string>Side;                //куда мы двигались чтобы прийти в эту позицию
    Used.insert(start);
    Queue.push(start);
    Father[start] = start;                              //добавляем первую вершину
    while (!Queue.empty())
    {
        Position regular = Queue.front();
        Queue.pop();
        std::vector<Position>Children;
        std::vector<std::string>Side_;
        regular.GetNeighbour(Children, Side_);                //получаем соседние позиции
        for(int i = 0; i < Children.size(); ++i)
        {
            if(Children[i].Square == 12345678)                  //проверяем не решилась ли восьминашка
            {
                Side[Children[i]] = Side_[i];                   //
                Sequence.push(Side[Children[i]]);               //тогда записываю ответ
                while (regular.Square != start.Square)
                {
                    Sequence.push(Side[regular]);
                    regular = Father[regular];
                }
                return Sequence;
            }
            else if(Used.count(Children[i]) == 0)               //иначе кидаю в очередь и ищу дальше
            {
                Queue.push(Children[i]);
                Used.insert(Children[i]);
                Father[Children[i]] = regular;
                Side[Children[i]] = Side_[i];
            }
        }
        
    }
    return Sequence;
}

int main()
{
    int first = 0;
    int digit = 0;
    std::vector<int>chenge;
    for(int i = 0; i < 9; ++i)                              //тут читаем просто и приводим в удобный вид
    {
        int cell = 0;
        std::cin >> cell;
        if(cell != 0)
        {
            chenge.push_back(cell);
        }
        first *= 10;
        first += (cell + 8) % 9;
        if(cell == 0)
        {
            digit = 8 - i;
        }
    }
    int chen = 0;
    for(int i = 0; i < 7; ++i)
    {
        for(int j = i + 1; j < 8; ++j)
        {
            if(chenge[i] > chenge[j])
            {
                ++chen;
            }
        }
    }
    if(chen % 2 == 1)
    {
        std::cout << -1;
    }
    else
    {
        Position start(first, digit);
        std::stack<std::string>Sequence = BFS(start);
        int size = Sequence.size();
        std::cout << size << std::endl;
        for(int i = 0; i < size; ++i)
        {
            std::cout << Sequence.top();
            Sequence.pop();
        }
    }
    return 0;
}
