using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Prism.Events;
using Viewer.AdvancedTypes.DockingToolHelper;
using Viewer.Interfaces;
using ICSharpCode.AvalonEdit.Document;
using Viewer.Infrastructure;

namespace Viewer.Tools.ViewModels
{
    public class OutputPaneViewModel : ToolViewModel
    {
        TextDocument _document;

        public OutputPaneViewModel() : base("Output")
        {
            MinHeight = 200;
            _document = new TextDocument();

            
        }


        IApplicationData _data;
        IEventAggregator _eventaggre;


        public TextDocument Document
        {
            get
            {
                return _document;
            }

            set
            {
                _document = value;
                RaisePropertyChanged();
            }
        }

        public override bool Connect(IApplicationData data, IEventAggregator eventaggre)
        {
            _data = data;
            _eventaggre = eventaggre;
            _eventaggre.GetEvent<OutputPrintEvent>().Subscribe(OnOutputPrintMessageArrived);
            return true;
        }

        private void OnOutputPrintMessageArrived(string obj)
        {
            _document.Text = _document.Text + Environment.NewLine + obj;
        }

        public override bool Disconnect()
        {
            return true;
        }
    }
}
