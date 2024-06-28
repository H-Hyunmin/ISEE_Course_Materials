void cleanBuffer(char *buf,int len)
{
  for(int i = 0;i < len;i++)
  {
    buf[i] = '\0';
  } 
}