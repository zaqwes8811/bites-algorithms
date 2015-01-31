
"""
# http://www.geeksforgeeks.org/to-find-smallest-and-second-smallest-element-in-an-array/#comment-2541
Find 2nd min in (n+lgn-2).

http://users.csc.calpoly.edu/~dekhtyar/349-Spring2010/lectures/lec03.349.pdf

struct minimum
{
int m1, m2;
};

minimum find_both_min(int a[], int p, int r) {
  minimum bmin;
  if(r-p==1)
  return a[p]<a[r]?(bmin.m1=a[p], bmin.m2=a[r]):(bmin.m1=a[r], bmin.m2=a[p]), bmin;
  int q = (p+r)/2;

  minimum lmin, rmin;
  lmin=find_both_min(a, p,q);
  rmin=find_both_min(a, q+1, r);
  if(lmin.m1<rmin.m1)
  {
    bmin.m1=lmin.m1;
    if(lmin.m2<rmin.m1)
    bmin.m2=lmin.m2;
    else bmin.m2=rmin.m1;
  }
  else
  {
    bmin.m1=rmin.m1;
    if(rmin.m2<lmin.m1)
    bmin.m2=rmin.m2;
    else bmin.m2=lmin.m1;
  }
  cout<<bmin.m1<<bmin.m2<<"n";
  return bmin;
}
"""
