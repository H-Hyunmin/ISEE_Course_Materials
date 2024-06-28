#line 1 "C:\\Users\\Huang Xianmin\\Desktop\\物联网系统设计\\课程实验与相关资料\\lab4\\threecolor_light\\connect.cpp"
void cleanBuffer(char *buf,int len)
{
  for(int i = 0;i < len;i++)
  {
    buf[i] = '\0';
  } 
}