﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Viewer.Tools.Views
{
    /// <summary>
    /// Interaction logic for LightCreatorPanel.xaml
    /// </summary>
    public partial class LightCreatorPanel : UserControl
    {
        public LightCreatorPanel()
        {
            try
            {
                InitializeComponent();
            }
            catch (Exception ex)
            {

                throw;
            }
        }

        
    }
}
