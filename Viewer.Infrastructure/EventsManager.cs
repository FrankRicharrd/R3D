using Prism.Events;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces;

namespace Viewer.Infrastructure
{

    public class NodeSelectionChangedEvent : PubSubEvent<IRNode> { }

    public class OnContextCreatedEvent : PubSubEvent<IRNode> { }

    public class StatusBarMessageEvent : PubSubEvent<string> { }

    public class ActiveIDocumentChangedEvent : PubSubEvent<IRDocument> { }

    public class NewDocumentAddedEvent : PubSubEvent<IRDocument> { }

    public class ApplicationLoadedEvent : PubSubEvent { }

    public class DockableToolToggleButtonClickedEvent : PubSubEvent<KeyValuePair<Viewer.Interfaces.UI.IRibbonToggleButton, Type>>{}

    public class OnTransformNodeSelectedOnScreen : PubSubEvent<IRTransformNode> { }


    public class OutputPrintEvent : PubSubEvent<string> { }

    public class OnKeyPress : PubSubEvent<int> { }
    public class OnKeyRelease : PubSubEvent<int> { }



}
