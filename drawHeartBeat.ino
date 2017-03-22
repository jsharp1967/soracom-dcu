void drawHeartBeat()
{
  //IP Address of the PI
  ucg.setColor(255, 255, 0);
  ucg.drawString(145,5,0,heartBeat.c_str());
  ucg.setColor(255, 255, 255);
}
