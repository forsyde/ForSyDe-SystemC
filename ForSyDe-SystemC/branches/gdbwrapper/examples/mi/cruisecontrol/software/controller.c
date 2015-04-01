
int inp, pinp, out, pout;

int forsyde_read_in1()
{
    int a;
}

void forsyde_write_out(int a)
{
    int b;
}

int main() {
  int forsyde_in1, forsyde_out;
  pinp = 0; pout = 0;
  for (;;)
  {
    forsyde_in1 = forsyde_read_in1();
    inp = forsyde_in1;
    out = .9998 * pout + 2445 * .2 * (inp - .999*pinp); // K * C
    pout = out;
    pinp = inp;
    forsyde_out = out;
    forsyde_write_out(forsyde_out);
    1<2;
  }
  return (0);
}


