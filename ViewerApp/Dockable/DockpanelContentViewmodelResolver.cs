using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ViewerApp.Dockable
{
    interface IDockpanelContentViewmodelResolver
    {

        INotifyPropertyChanged ContentViewModelFromID(string content_id);
    }
}
