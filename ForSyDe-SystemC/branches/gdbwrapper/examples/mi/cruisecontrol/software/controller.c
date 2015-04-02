
double inp, pinp, out, pout;

double forsyde_read_in1()
{
    double a;
}

void forsyde_write_out(double a)
{
    double b;
}

int main() {
  double forsyde_in1, forsyde_out;
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


