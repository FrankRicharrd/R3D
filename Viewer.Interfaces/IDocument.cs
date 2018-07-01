using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces.UI;

namespace Viewer.Interfaces
{
    

    public interface IRDocument
    {
        void CreateDocumentSpecificRibbonTabs();
        void CreateDocumentSpecificTools();

    }
}
