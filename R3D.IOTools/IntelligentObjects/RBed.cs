using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces;
using Viewer.Interfaces.IntelligentObject;

namespace R3D.IOTools.IntelligentObjects
{
    public class RBed : IRIntelligentObjectPlugin
    {
        string _iconPath;
        public string IconPath
        {
            get
            {
                return _iconPath;
            }

            set
            {
                _iconPath = value;
            }
        }

        string _name;
        public string Name
        {
            get
            {
                return _name;
            }

            set
            {
                _name = value;
            }
        }
        IApplicationData _data;
        object _IOManager;
        public bool Connect(IApplicationData data, object IOManager)
        {
            _data = data;
            _IOManager = IOManager;
            IconPath = data.GetIconPath("bed.png");
            return true;
        }

        public bool CreateObject()
        {
            return true;
        }
    }
}
