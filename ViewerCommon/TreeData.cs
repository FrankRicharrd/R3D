using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces;
using ViewerCommon.Model;

namespace ViewerCommon
{
    public class TreeData : ITree
    {
        private IApplicationData appData;

        RBuilderDocument _document;

        IRTransformNode _geometryParentNode;

        public TreeData(IApplicationData appData, RBuilderDocument doc)
        {
            this.appData = appData;
            _document = doc;
        }

        public RBuilderDocument Document
        {
            get
            {
                return _document;
            }

            set
            {
                _document = value;
            }
        }

        public IRTransformNode GeometryParentNode
        {
            get
            {
                return _geometryParentNode;       
            }

            set
            {
                _geometryParentNode = value;
            }
        }
        
    }
}
