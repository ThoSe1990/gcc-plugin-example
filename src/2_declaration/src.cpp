int some_variable = 1;

const char* p_c = "hello world";
const char* const p_cp = "hello world";
const char* c_arr[] = {"first", "second", "third"};

int i = 99;
int& ref = i;
int const ci = 123;
const unsigned int cui = 123;

long l;
long long ll;

float fl = 1.1f;
double d = 1.12;
unsigned int ui = 123;
const double cd = 1;

int* p_i;
int i_arr[] = {1, 2, 3, 4};
const int ci_arr[] = {1, 2, 3, 4};

namespace cwt
{
  int value = 1;
  namespace nested
  {
    int other_value = 2; 
  } // namespace nested 
}

// struct foo
// {
// };
// foo f;
// foo* p_f;
// foo& ref_foo = f;
// foo foo_arr[3];

// struct foo_with_value
// {
//   int m_val;
// };
// foo_with_value fwv_1;
// foo_with_value fwv_2{99};

// struct foo_with_default
// {
//   int m_value = 11;
// };
// foo_with_default fwd_1;
// foo_with_default fwd_2{99};

// struct
// {
// } unnamed;

// void* v;
// void (*function_pointer)();

// union bar
// {
//   int i;
//   double d;
//   float f;
// };

// bar b;

// namespace n
// {
//   foo f;
//   int i = 123;
// }

// void func();
// void func()
// {
//   int local1;
//   int local2 = 234;
//   foo f;
// }

// void func_args(int arg);

// template<typename T>
// void t_foo(T a, T b)
// {

// }

// void func_args(int arg)
// {
//   t_foo(arg, arg);
// }
// auto a = 1

// template<typename T>
// struct templated
// {
//   T m_value;
// };
// templated<int> templated_int{12};