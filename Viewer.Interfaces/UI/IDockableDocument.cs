using Prism.Events;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Viewer.Interfaces.UI
{
    public interface IDockableDocument
    {

        bool Connect(IApplicationData data, IEventAggregator events);

    }
}
