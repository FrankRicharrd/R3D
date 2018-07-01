using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces;
using Viewer.Interfaces.UI;

namespace Viewer.AdvancedTypes.Document
{
    public abstract class RDocument : IRDocument
    {
        IApplicationData _data;

    
        dynamic _editor;

        ITree _tree;


        public dynamic Editor
        {
            get
            {
                return _editor;
            }

            set
            {
                _editor = value;
            }
        }
        
        public IApplicationData Data
        {
            get
            {
                return _data;
            }

            set
            {
                _data = value;
            }
        }

        protected RDocument(IApplicationData data)
        {
            Data = data;     
        }

      
        public abstract void CreateDocumentSpecificRibbonTabs();

        public abstract void CreateDocumentSpecificTools();

       
    }
}
