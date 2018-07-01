using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Prism.Events;
using Viewer.Interfaces;
using Viewer.Interfaces.UI;

namespace Viewer.AdvancedTypes.DockingToolHelper
{
    public abstract class DocumentViewModel : PaneViewModel, IDockableDocument
    {

        string _contenttext = "sampleText";

        IRDocument _rdocument;

        public DocumentViewModel(IRDocument rdoc)
        {
            _rdocument = rdoc;

            ContentId = "Document1";

            Title = "Document-created";
        }

        public string Contenttext
        {
            get
            {
                return _contenttext;
            }

            set
            {
                _contenttext = value;
                RaisePropertyChanged();
            }
        }

        public IRDocument Rdocument
        {
            get
            {
                return _rdocument;
            }

            set
            {
                _rdocument = value;
            }
        }

        public abstract bool Connect(IApplicationData data, IEventAggregator events);
        
    }
}
