using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace AndroidProjectMaker
{
    public partial class mainform : Form
    {
        public mainform()
        {
            InitializeComponent();

            textBox1.SelectionLength = 0;
        }


        public void setMessage(string msg)
        {
            this.Text = msg;
            //this.Invalidate(true);
        }
    }
}
