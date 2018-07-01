using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using Viewer.AdvancedTypes.DockingToolHelper;

namespace ViewerApp.Dockable
{
    public class PanesTemplateSelector : DataTemplateSelector
    {
        public PanesTemplateSelector()
        {

        }


        public DataTemplate FileViewTemplate
        {
            get;
            set;
        }

        public DataTemplate FileStatsViewTemplate
        {
            get;
            set;
        }

        public override System.Windows.DataTemplate SelectTemplate(object item, System.Windows.DependencyObject container)
        {
            if (item == null)
                return null;

            Type tt = item.GetType();
            DataTemplateKey k = new DataTemplateKey(tt);

            try
            {

                object resource = Application.Current.FindResource(k);

                if (resource != null && resource is DataTemplate)
                {
                    return resource as DataTemplate;
                }

            }
            catch (ResourceReferenceKeyNotFoundException ex)
            {
                var pane = item as PaneViewModel;

                if (string.IsNullOrEmpty(pane.SourceUri))
                    return base.SelectTemplate(item, container);


                ResourceDictionary dic = new ResourceDictionary();
                dic.Source = new Uri(pane.SourceUri,
                UriKind.RelativeOrAbsolute);

                try
                {
            

                    var dt = dic[k] as DataTemplate;

                    Application.Current.Resources.MergedDictionaries.Add(dic);

                    if (dt == null)
                    {
                        return base.SelectTemplate(item, container);
                    }

                    return dt;


                }
                catch (ResourceReferenceKeyNotFoundException exa)
                {

                    return base.SelectTemplate(item, container);

                }
            }


            return base.SelectTemplate(item, container);
        }
    }
}
