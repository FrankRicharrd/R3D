using Prism.Events;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ViewerApp.CustomEvents
{
    /// <summary>
    /// Class implements a simple PRISM LoadLayout string event
    /// </summary>
    public class LoadLayoutEvent : PubSubEvent<string>
    {
        private static readonly EventAggregator _eventAggregator;
        private static readonly LoadLayoutEvent _event;

        static LoadLayoutEvent()
        {
            _eventAggregator = new EventAggregator();
            _event = _eventAggregator.GetEvent<LoadLayoutEvent>();
        }

        public static LoadLayoutEvent Instance
        {
            get { return _event; }
        }
    }
}
