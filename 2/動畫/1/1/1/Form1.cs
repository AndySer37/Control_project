using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms; 
namespace _1
{
    public partial class Form1 : Form
    {
        Image spider;      // image of spider
        Image spider2;      // image of spider
        float Angle = 0;  // 擺盪的角度
        float Angle2 = 0;  // 擺盪的角度
        double[] phi=new double[502];
        double[] theta = new double[502];
        float Angle_D = 1; // 角度的遞增值
        int spider_y = 100; // 之豬的y軸高度	
       // Pen myPen = new Pen(Color.DarkGray, 2); // 汁豬思
      //  int kukuIndex = 0,orindex = 0;
        int a=100, b=400, timer1_t=0, xx;

      

        public Form1()
        {
            InitializeComponent();
            spider = new Bitmap(Properties.Resources.車子1);
            spider2 = new Bitmap(Properties.Resources.輪子);
            int counter = 0;
            int success;
            string line;

            // Read the file and display it line by line.  
            System.IO.StreamReader file =
                new System.IO.StreamReader("D:\\google_download\\自動控制\\Lab02\\動畫\\1\\1\\Resources\\test4.txt");
            while ((line = file.ReadLine()) != null)
            {
                System.Console.WriteLine(line);
                counter++; 
                phi[counter] =Double.Parse(line);
                System.Console.WriteLine(phi[counter]);
            }

            file.Close();
            counter = 0;
            System.IO.StreamReader file2 =
               new System.IO.StreamReader("D:\\google_download\\自動控制\\Lab02\\動畫\\1\\1\\Resources\\testtheta4.txt");
            while ((line = file2.ReadLine()) != null)
            {
                System.Console.WriteLine(line);
                counter++;
                theta[counter] = Double.Parse(line);
                System.Console.WriteLine(phi[counter]);
            }

            file2.Close();

        }

        private void Form1_Paint(object sender, PaintEventArgs e)
        {
            int x = this.ClientSize.Width / 2;
            int y = this.ClientSize.Height / 2;
            Bitmap scr = new Bitmap(820, 640);  // whole screen image
            Graphics dr1 = Graphics.FromImage(scr);  // 圖層1
            Graphics dr2 = Graphics.FromImage(scr);  // 圖層2
            Graphics dr3 = Graphics.FromImage(scr);  // 圖層3


            x += xx;
            //平移畫布的原點
            dr1.TranslateTransform(x+ 50, y+ 100);
            //旋轉畫布
            dr1.RotateTransform(Angle);
            // dr1.TranslateTransform(0, -(spider_y + spider.Height / 2));
            //化出汁豬的影像 
            dr1.DrawImage(spider, -50, -300 ,100, 200);
           

            dr2.TranslateTransform(x + 50, y + 50);
            dr2.RotateTransform(Angle2);
            dr2.DrawImage(spider2, -50, -50, 100, 100);
            //化出汁豬思
            //dr1.DrawLine(myPen, 0, 0, 0, spider_y + 32);

            //畫出歐噴哭哭
            /*dr2.DrawImage(openkuku.Images[kukuIndex], 100, 400);

            if (Control.MouseButtons == System.Windows.Forms.MouseButtons.Left)
            {
                k = 1;
                timer3.Enabled=true;
            }

            if (k == 1)
            {
                dr3.DrawImage(orange.Images[orindex], a, b);
            }
            */


            e.Graphics.DrawImage(scr, 0, 0);
        }

        private void timer1_Tick(object sender, EventArgs e)
        {
            // Angle = Angle + Angle_D;　// 更新擺盪的角度
            // if (Angle > 60 || Angle < -60)
            //     Angle_D = -Angle_D;
            // xx =(int) (20*Angle*3.14/180);
            Angle = (float)(phi[timer1_t]*180/3.14);
            Angle2 = (float)(theta[timer1_t] * 180 / 3.14);
            xx = (int)(200 * Angle2 * 3.14 / 180);
            timer1_t++;
            this.Invalidate(); // 重畫
            
        }

        private void timer2_Tick(object sender, EventArgs e)
        {
            //Angle2 = Angle2 + 5*Angle_D;　// 更新擺盪的角度
            //if (Angle2 > 360 || Angle2 < -360)
            //    Angle_D = -Angle_D;
            //Angle2 = (float)(theta[timer1_t] * 180 / 3.14);
           // xx = (int)(20 * Angle2 * 3.14 / 180);
            
           // this.Invalidate();
        } // 重畫
                               /* kukuIndex++;
                                if (kukuIndex == openkuku.Images.Count)
                                    kukuIndex = 0;

                                this.Invalidate(); // 重畫
        }

        private void timer3_Tick(object sender, EventArgs e)
        {
            Point pt1 = Control.MousePosition;
            pt1 = this.PointToClient(pt1);
            c=pt1.X-100;
            d=400-pt1.Y;
            a+=c/10;
            b-=d/10;
            orindex++;
            if (orindex == orange.Images.Count)
                orindex = 0;
            if (a > 800 || b < 0)
            {
                a = 100;
                b = 400;
                k = 0;
                timer3.Enabled = false;
            }

            this.Invalidate();
        }

       */
    }
}
