using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces;

namespace Viewer.Tools
{
    public class InternalToolsPlugin : IRPlugin
    {
        public void Connect(IApplicationData data)
        {
           
        }

        public List<RDocumentData> GetDocumentData()
        {
            return null;
        }

        public string GetXAMLResourcePath()
        {
            return "pack://application:,,,/Viewer.Tools;component/Resource/ToolsResourceDictionary.xaml";
        }
    }
}
