#include "unit.h"
#include <iostream>

std::vector<int> Schneiden(const std::vector<int>& a, const std::vector<int>& b)
{
    std::vector<int> res;
    for (size_t i = 0, j = 0; i < a.size() && j < b.size(); i++)
    {
        while(j < b.size() && a[i] > b[j])
            j++;
        if(a[i] < b[j])
            continue;
        if(a[i] == b[j])
            res.push_back(a[i]);
    }
    return res;
}

std::vector<int> Vereinigen(const std::vector<int>& a, const std::vector<int>& b)
{
    std::vector<int> res;
    if(a.size() == 0)
        res.insert(res.begin(), b.begin(), b.end());
    else if(b.size() == 0)
        res.insert(res.begin(), a.begin(), a.end());
    else
    {
        size_t i = 0, j= 0;
        for (; i < a.size() && j < b.size();) {
            if(a[i] < b[j])
                res.push_back(a[i++]);
            else if(b[j] < a[i])
                res.push_back(b[j++]);
            else // a[i] == b[j]
            {
                if(res.size() == 0 || res.back() != a[i]);
                    res.push_back(a[i++]);
                j++;
            }
        }
        // fill it up
        while (i < a.size())
            res.push_back(a[i++]);
        while (j < b.size())
            res.push_back(b[j++]);
    }
    return res;
}

bool TestFirst(std::vector<std::vector<int>>& M)
{
    bool empty = false;
    bool containsG = false;

    for(auto& val: M)
    {
        if(val.size() == 0)
            empty = true;
        else if(val.size() < 10)
        {
            for (size_t i = 0; i < val.size(); i++)
                if(val[i] >= 10)
                    return false;
        }
        else if(val.size() == 10)
        {
            for (size_t i = 0; i < val.size(); i++)
                if(val[i] >= 10)
                    return false;
            containsG = true;
        }
    }
    return containsG && empty;
}

bool TestSecond(std::vector<std::vector<int>>& M, size_t maxIter)
{
    int it = 0;

    for(auto& set: M)
    {
        for(auto& set2: M)
        {
            if(set == set2)
                continue;

            auto schnitt = Schneiden(set, set2);
            bool contains = false;
            for(auto& set3: M)
            {
                if(schnitt.size() != set3.size())
                    continue;

                bool _contains = true;
                for(size_t i = 0; i < schnitt.size(); i++)
                {
                    if(schnitt[i] != set3[i])
                    {
                        _contains = false;
                        break;
                    }
                }
                if(_contains)
                {
                    contains = true;
                    break;
                }
            }
            if(!contains)
                return false;
            it++;
            if(it == maxIter)
                return true; // found no mismatches so far
        }
    }
    return true;
}


bool TestThird(std::vector<std::vector<int>>& M, size_t maxIter)
{
    int it = 0;

    for(auto& set: M)
    {
        for(auto& set2: M)
        {
            if(set == set2)
                continue;

            auto einigung = Vereinigen(set, set2);
            bool contains = false;
            for(auto& set3: M)
            {
                if(einigung.size() != set3.size())
                    continue;

                bool _contains = true;
                for(size_t i = 0; i < einigung.size(); i++)
                {
                    if(einigung[i] != set3[i])
                    {
                        _contains = false;
                        break;
                    }
                }
                if(_contains)
                {
                    contains = true;
                    break;
                }
            }
            if(!contains)
                return false;
            it++;
            if(it == maxIter)
                return true; // found no mismatches so far
        }
    }
    return true;
}

// bool TestFuncTopo(std::vector<std::vector<int>>& M, std::vector<int>& f)
// {
//     auto()
// }
//
bool TestFunc(std::vector<std::vector<int>>& M, std::vector<int>& f)
{
    auto fInv = [&](std::vector<int> set)
    {
        std::vector<int> res;
        for (auto& e: set)
        {
            for (size_t i = 0; i < f.size(); i++) {
                if(f[i] == e)
                {
                    res = Vereinigen(res, {i});
                    // std::cout << e  << " " << i << " " << res.size()<< '\n';
                }
            }
        }
        return res;
    };

    auto abb = fInv({5});

    std::cout << "Test: {";
    for(auto& val: abb)
    {
        std::cout << val << ", ";
    }
    std::cout << "}" << '\n';
    // abb = fInv(M[0]);
    //
    // std::cout << "Test2: {";
    // for(auto& val: abb)
    // {
    //     std::cout << val << ", ";
    // }
    // std::cout << "}" << '\n';
}

int main()
{
    size_t i = 1;
    // for (size_t i = 1; i <= AnzahlMengensysteme; i++)
    {
        std::vector<std::vector<int>> M;
        Mengensystem(i, M);
        bool isTopo = TestFirst(M);
        if(isTopo)
            isTopo = TestSecond(M, 5000);
        if(isTopo)
            isTopo = TestThird(M, 5000);
        std::cout << i << " " << isTopo << '\n';

        // for(auto& set: M)
        // {
        //     std::cout << i <<": {";
        //     for(auto& val: set)
        //     {
        //         std::cout << val << ", ";
        //     }
        //     std::cout << "}" << '\n';
        // }
        // if(!isTopo)
        //     continue;

        std::vector<int> abb;
        Abbildung(1, abb);
        Ergebnis(i, 1, true);
        Abbildung(2, abb);
        // if(i != 5)
            Ergebnis(i, 2, false);
        // else
        //     Ergebnis(i, 2, true);
        //
        // // if(i != 1 && i != 4)
        // // {
        // //     Abbildung(3, abb);
        // //     Ergebnis(i, 3, false);
        // //
        // // }
        Abbildung(4, abb);
        // if(i != 5)
            Ergebnis(i, 4, true);
        // else
        //     Ergebnis(i, 4, true);
        Abbildung(5, abb);
        // if(i != 5)
            Ergebnis(i, 5, false);
        // else
        //     Ergebnis(i, 5, true);
        for (size_t j = 1; j <= AnzahlAbbildungen; j++)
        {
            std::vector<int> abb;
            Abbildung(j, abb);

            std::cout << i << ", " << j <<": {";
            for(auto& val: abb)
            {
                std::cout << val << ", ";
            }
            std::cout << "}" << '\n';
            TestFunc(M, abb);
        }
    }

    return 0;
}
