using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Viewer.Interfaces;
using Viewer.Interfaces.UI;

namespace Viewer.Commands
{
    public class LightCommands 
    {
        IApplicationData _data;
        IRLightManager _manager;
        public LightCommands(IApplicationData data, IRLightManager manager)
        {
            Data = data;
            _manager = manager;

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

        public void CreateLightTab(string typename)
        {
            var lightsTab = Data.UIManager.CreateNewTab(typename, "Lights");

            var lightssettingsgroup = lightsTab.CreateButtonGroup("Light_Settings");

            var lightEnable = lightssettingsgroup.CreateToggleButton("On_OFF", Data.GetIconPath("lightonoff.png"), new RelayCommand<IRibbonToggleButton>(MainLightsOnorOFF));

            var headlight= lightssettingsgroup.CreateToggleButton("HeadLight", Data.GetIconPath("headlight.png"), new RelayCommand<IRibbonToggleButton>(headlightswitch));

            var skylight = lightssettingsgroup.CreateToggleButton("SkyLight", Data.GetIconPath("skylight.png"), new RelayCommand<IRibbonToggleButton>(skylightswitch));

            var extraLights = lightssettingsgroup.CreateToggleButton("ExtrasLights", Data.GetIconPath("skylight.png"), new RelayCommand<IRibbonToggleButton>(extraslightswitch));

        }

        private void extraslightswitch(IRibbonToggleButton obj)
        {
            _manager.ExtraLights((obj.IsChecked == true) ? true : false);
        }

        private void skylightswitch(IRibbonToggleButton obj)
        {
            _manager.SkyLightSwitch((obj.IsChecked == true) ? true : false);

        }

        private void headlightswitch(IRibbonToggleButton obj)
        {
            _manager.HeadLightSwitch((obj.IsChecked == true) ? true : false);
        }

        private void MainLightsOnorOFF(IRibbonToggleButton obj)
        {
            _manager.SwitchMainLight((obj.IsChecked == true) ? true : false);
        }
    }
}
