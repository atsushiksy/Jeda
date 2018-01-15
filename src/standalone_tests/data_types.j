/*
 * test arrays and lists
 */
void main() 
{
   int me[], i;
   int mee[];
   int foo[100], foo1[100];
   bit [300:1] bar[], bar1[];
   float  flt<>;
   int afoo[100][30][10];
   int myary[*];
   int int_list<>, tmp_list<>;
   string str_list<>;
   
   string mystr="a";
   
   // myary[mystr]="abc";  // string becomes an integer??

   // test string indexed array
   mystr = "next_movie";
   myary[mystr]=1;

   bar[1] = 301'h123;
   bar[1000] = 301'h123;

   // foo[101] = "cde";
   foo[1] = 123;

   afoo[99][0][0] = 101;

   printf("foo size=%0d\n", foo.num_element());

   // try array assignment
   foo1 = foo;

   bar1 = bar;
   me[1] = 1;
   mee = me;

   i = foo.num_element();
   if(afoo.dimension()!=3)  
     error("Error: diminsion number %d error\n",afoo.dimension());
   if(afoo.dim_size(0)!=100) error("Error: dimsize(1) number  error\n");
   if(afoo.dim_size(1)!=30) error("Error: dimsize(2) number  error\n");
   if(afoo.dim_size(2)!=10) error("Error: dimsize(3) number  error\n");

// NOT    i = myary.dimension();
// NOT    i = me.dimension();

// Not    i = foo.num_element(1);  
// Not    i = me.dim_size(1);  
// Not    i = myary.dim_size();

    i = foo.num_element();
    i = afoo.num_element();

    i = me.num_element();
    i = myary.num_element();

    str_list = myary.index_list();   
    tmp_list = me.index_list();   

    // check_index returns 1 if ture??
    bar[1] = 101;
    if(bar.check_index(1) ) printf("right in check_index\n");
    else                    error("error in check_index\n");
    myary["a"] = 123;
    if(myary.check_index("a") ) printf("right in check_index\n");
    else                        error("error in check_index\n");
    // delete_index()
    bar.delete_index(1);
    // bar.delete_index(0);
    myary.delete_index("a");
    //myary.delete_index("b");

    // flush all
    bar.flush_all();
    myary.flush_all();

    /*
     *  list test
     */
    int_list.push(1);
    int_list.push(2);
    if(int_list.pop()!=2) error("pop value error\n");
    if(int_list.pop()!=1) error("pop value error\n");
    // i = int_list.pop();
    int_list.bpush(100);
    int_list.bpush(101);
    int_list.sort();
    if(int_list.bpop()!=101) error("pop value error\n");
    if(int_list.bpop()!=100) error("pop value error\n");


    flt.push(1.234);
    printf("list flt: number entry %0d\n", flt.num_element());
    flt.push(1);
    flt.push(2);
    if(flt.pop()!=2) error("pop value error\n");
    if(flt.pop()!=1) error("pop value error\n");
    i = flt.pop();
    flt.bpush(100);
    flt.bpush(101);
    flt.sort();
    if(flt.bpop()!=101) error("pop value error\n");
    if(flt.bpop()!=100) error("pop value error\n");

    // string
    str_list.push("1");
    str_list.push("2");
    if(str_list.pop()!="2") error("pop value error\n");
    if(str_list.pop()!="1") error("pop value error\n");
    printf("pop value %s\n",  str_list.pop());
    str_list.bpush("100");
    str_list.bpush("101");
    str_list.sort();
    if(str_list.bpop()!="101") error("pop value error\n");
    if(str_list.bpop()!="100") error("pop value error\n");
}
