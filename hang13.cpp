// /home/martin/compiler/gcc/gcc/testsuite/g++.dg/cpp0x/bracket1.C

template<typename T>
struct vector
{
    operator T() const;
};

void f()
{
    vector<vector<int>> v;
    const vector<int> vi = static_cast<vector<int>>(v);
}
