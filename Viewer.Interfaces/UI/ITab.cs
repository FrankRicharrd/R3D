using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viewer.Interfaces.UI
{
    public interface ITab
    {
        List<IButtonGroup> ButtonGroups { get; set; }




        string TabName { get; set; }

        IButtonGroup CreateButtonGroup(string name);

        IButtonGroup GetButtonGroup(string name);


    }
}
