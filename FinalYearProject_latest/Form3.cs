using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using WMPLib;


namespace FinalYearProject_latest
{

    public partial class video : Form
    {
        
        [DllImport("C:\\Users\\Sunny Patel\\Documents\\Visual Studio 2012\\Projects\\DLL.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern int eyetrack(int flag);
        public video()
        {
            InitializeComponent();
        }

        private void open_Click(object sender, EventArgs e)
        {
            if (openfile.ShowDialog() == DialogResult.OK)
                //axVLCPlugin1.addTarget("file:///" + openfile.FileName, null, AXVLC.VLCPlaylistMode.VLCPlayListReplaceAndGo, 0);
                player.URL = @openfile.FileName;
                      
            
                //player.Ctlcontrols.pause();
        }

        private void play_Click(object sender, EventArgs e)
        {
            
          //  pause.Visible = true;
           // axVLCPlugin1.play();
           // player.Ctlcontrols.play();
            int run = 1;
            if (run != 0)
            {
                while (run != 0)
                {

                    try
                    {
                        run = eyetrack(10);

                    }
                    catch (Exception c) { Console.WriteLine("Exception:" + c); }

                }
                player.Ctlcontrols.pause();


            }
            else
            {
                player.Ctlcontrols.pause();
                run = 1;
            }
           // System.Threading.Thread.Sleep(1000);
       }


        private void pause_Click(object sender, EventArgs e)
        {
            player.Ctlcontrols.pause();
            //btn_pause.Visible = false;
        }
          
    }
}