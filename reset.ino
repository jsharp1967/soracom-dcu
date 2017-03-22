void reset()
{
  ucg.clearScreen();
  ucg.setRotate270();
  ucg.setFont(ucg_font_7x14B_mf);
  ucg.setColor(255, 255, 255);
  ucg.setColor(255, 255, 255);
  //ucg.drawFrame(0, 0, 160, 44);
  // Draw the buttons
  ucg.setColor(255, 255, 255);
  ucg.drawFrame(0,111, 66,17);
  ucg.drawFrame(94,111, 66,17);
  ucg.setColor(0, 255, 0);
  ucg.drawString(12,125,0,"UPLOAD");
  ucg.drawString(100,125,0,"DOWNLOAD");
  ucg.setColor(255, 255, 255);
}

