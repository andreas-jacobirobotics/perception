#include <iostream>

#include "phoxi_sensor.h"

PhoxiSensor::PhoxiSensor(const std::string& frame, const std::string& device_name, const std::string& size) : frame(frame), device_name(device_name), size(size), running(false) {

}

bool PhoxiSensor::start() {
    if (!connect()) {
        running = false;
        return false;
    }

    running = true;
    return true;
}

bool PhoxiSensor::connect() {
    pho::api::PhoXiFactory Factory;

    //Check if the PhoXi Control Software is running
    if (!Factory.isPhoXiControlRunning())
    {
        std::cout << "PhoXi Control Software is not running" << std::endl;
        return 0;
    }

    //Get List of available devices on the network
    std::vector <pho::api::PhoXiDeviceInformation> DeviceList = Factory.GetDeviceList();
    if (DeviceList.empty())
    {
        std::cout << "PhoXi Factory has found 0 devices" << std::endl;
        return 0;
    }
    printDeviceInfoList(DeviceList);

    //Try to connect device opened in PhoXi Control, if any
    PhoXiDevice = Factory.CreateAndConnectFirstAttached();
    if (PhoXiDevice)
    {
        std::cout << "You have already PhoXi device opened in PhoXi Control, the API Example is connected to device: "
            << (std::string) PhoXiDevice->HardwareIdentification << std::endl;
    }
    else
    {
        std::cout << "You have no PhoXi device opened in PhoXi Control, the API Example will try to connect to last device in device list" << std::endl;
        PhoXiDevice = Factory.CreateAndConnect(DeviceList.back().HWIdentification);
    }

    //Check if device was created
    if (!PhoXiDevice)
    {
        std::cout << "Your device was not created!" << std::endl;
        return 0;
    }

    //Check if device is connected
    if (!PhoXiDevice->isConnected())
    {
        std::cout << "Your device is not connected" << std::endl;
        return 0;
    }

    PhoXiDevice->CapturingSettings->AmbientLightSuppression = true;
    std::cout << "PhoXiDevice->CapturingSettings->SinglePatternExposure " << PhoXiDevice->CapturingSettings->SinglePatternExposure << std::endl;
    // PhoXiDevice->CapturingSettings->SinglePatternExposure = PhoXiDevice->SupportedSinglePatternExposures[6];

    return false;
}

void PhoxiSensor::stop() {
    if (PhoXiDevice->isAcquiring())
    {
        PhoXiDevice->StopAcquisition();
    }
   
   PhoXiDevice->Disconnect(true, false);
}

void PhoxiSensor::frames() {
    if (PhoXiDevice->isAcquiring())
    {
        //Stop acquisition to change trigger mode
        PhoXiDevice->StopAcquisition();
    }

    PhoXiDevice->TriggerMode = pho::api::PhoXiTriggerMode::Software;
    std::cout << "Software trigger mode was set" << std::endl;
    PhoXiDevice->ClearBuffer();
    PhoXiDevice->StartAcquisition();
    if (!PhoXiDevice->isAcquiring())
    {
        std::cout << "Your device could not start acquisition!" << std::endl;
        return;
    }

    int FrameID = PhoXiDevice->TriggerFrame();
    if (FrameID < 0)
    {
        //If negative number is returned trigger was unsuccessful
        std::cout << "Trigger was unsuccessful! code=" << FrameID << std::endl;
        return;
    }
    else
    {
        std::cout << "Frame was triggered, Frame Id: " << FrameID << std::endl;
    }
    
    std::cout << "Waiting for frame " << std::endl;
    Frame = PhoXiDevice->GetSpecificFrame(FrameID, pho::api::PhoXiTimeout::Infinity);
    
    if (Frame)
    {
        const auto& cameraMatrix = Frame->Info.CameraMatrix;
        // std::vector<float> x;
        // x.push_back()
        fx = (float) cameraMatrix[0][0];
        fy = (float) cameraMatrix[1][1];
        cx = (float) cameraMatrix[0][2];
        cy = (float) cameraMatrix[1][2];
        printFrameInfo(Frame);
        printFrameData(Frame);
    }
    else
    {
        std::cout << "Failed to retrieve the frame!" << std::endl;
    }
    PhoXiDevice->StopAcquisition();
}

std::vector<std::vector<float>> PhoxiSensor::get_depth_map() {
    const int width = Frame->DepthMap.Size.Width;
    const int height = Frame->DepthMap.Size.Height;
    std::vector<std::vector<float>> depth_map;
    depth_map.resize(height);
    for (size_t i = 0; i < height; i++)
    {   
        depth_map[i].resize(width);

        for (size_t j = 0; j < width; j++)
        {
            depth_map[i][j] = Frame->DepthMap.At(i,j);
        }
        
    }

    return depth_map;
}

std::vector<std::vector<float>> PhoxiSensor::get_texture() {
    const int width = Frame->Texture.Size.Width;
    const int height = Frame->Texture.Size.Height;
    std::vector<std::vector<float>> texture;
    texture.resize(height);
    for (size_t i = 0; i < height; i++)
    {   
        texture[i].resize(width);

        for (size_t j = 0; j < width; j++)
        {
            texture[i][j] = Frame->Texture.At(i,j);
        }
        
    }

    return texture;
}

// std::vector<std::vector<float>> PhoxiSensor::get_point_cloud() {
//     const int width = Frame->PointCloud.Size.Width;
//     const int height = Frame->PointCloud.Size.Height;
//     std::vector<std::vector<float>> cloud;
//     cloud.resize(height);
//     for (size_t i = 0; i < height; i++)
//     {   
//         cloud[i].resize(width);

//         for (size_t j = 0; j < width; j++)
//         {
//             cloud[i][j] = Frame->PointCloud.At(i,j);
//         }
        
//     }

//     return cloud;
// }

// std::vector<std::vector<float>> PhoxiSensor::get_normal_map() {
//     const int width = Frame->NormalMap.Size.Width;
//     const int height = Frame->NormalMap.Size.Height;
//     std::vector<std::vector<float>> normal_map;
//     normal_map.resize(height);
//     for (size_t i = 0; i < height; i++)
//     {   
//         normal_map[i].resize(width);

//         for (size_t j = 0; j < width; j++)
//         {
//             normal_map[i][j] = Frame->NormalMap.At(i,j);
//         }
        
//     }

//     return normal_map;
// }

// std::vector<float> PhoxiSensor::get_depth_map_1d() {
//     const int width = Frame->DepthMap.Size.Width;
//     const int height = Frame->DepthMap.Size.Height;
//     std::vector<float> depth_map;
//     depth_map.resize(width * height);
//     for (size_t i = 0; i < height; i++)
//     {
//         for (size_t j = 0; j < width; j++)
//         {
//             depth_map[i*width + j] = Frame->DepthMap.At(i,j);
//         }
        
//     }

//     return depth_map;
// }

void PhoxiSensor::printDeviceInfoList(const std::vector<pho::api::PhoXiDeviceInformation> &DeviceList)
{
    for (std::size_t i = 0; i < DeviceList.size(); ++i)
    {
        std::cout << "Device: " << i << std::endl;
        printDeviceInfo(DeviceList[i]);
    }
}

void PhoxiSensor::printDeviceInfo(const pho::api::PhoXiDeviceInformation &DeviceInfo)
{
    std::cout << "  Name:                    " << DeviceInfo.Name << std::endl;
    std::cout << "  Hardware Identification: " << DeviceInfo.HWIdentification << std::endl;
    std::cout << "  Type:                    " << std::string(DeviceInfo.Type) << std::endl;
    std::cout << "  Firmware version:        " << DeviceInfo.FirmwareVersion << std::endl;
    std::cout << "  Variant:                 " << DeviceInfo.Variant << std::endl;
    std::cout << "  IsFileCamera:            " << (DeviceInfo.IsFileCamera ? "Yes" : "No") << std::endl;
    std::cout << "  Feature-Alpha:           " << (DeviceInfo.CheckFeature("Alpha") ? "Yes" : "No") << std::endl;
    std::cout << "  Feature-Color:           " << (DeviceInfo.CheckFeature("Color") ? "Yes" : "No") << std::endl;
    std::cout << "  Status:                  "
        << (DeviceInfo.Status.Attached ? "Attached to PhoXi Control. " : "Not Attached to PhoXi Control. ")
        << (DeviceInfo.Status.Ready ? "Ready to connect" : "Occupied")
        << std::endl << std::endl;
}

void PhoxiSensor::printFrameInfo(const pho::api::PFrame &Frame)
{
    const pho::api::FrameInfo &FrameInfo = Frame->Info;
    std::cout << "  Frame params: " << std::endl;
    std::cout << "    Frame Index: "                << FrameInfo.FrameIndex << std::endl;
    std::cout << "    Frame Timestamp: "            << FrameInfo.FrameTimestamp << " ms" << std::endl;
    std::cout << "    Frame Acquisition duration: " << FrameInfo.FrameDuration << " ms" << std::endl;
    std::cout << "    Frame Computation duration: " << FrameInfo.FrameComputationDuration << " ms" << std::endl;
    std::cout << "    Frame Transfer duration: "    << FrameInfo.FrameTransferDuration << " ms" << std::endl;
    std::cout << "    Frame Acquisition time (PTP): " << FrameInfo.FrameStartTime.TimeAsString("%Y-%m-%d %H:%M:%S") << std::endl;
    std::cout << "    Sensor Position: ["
        << FrameInfo.SensorPosition.x << "; "
        << FrameInfo.SensorPosition.y << "; "
        << FrameInfo.SensorPosition.z << "]"
        << std::endl;
    std::cout << "    Total scan count: "           << FrameInfo.TotalScanCount << std::endl;
    std::cout << "    Color Camera Position: ["
        << FrameInfo.ColorCameraPosition.x << "; "
        << FrameInfo.ColorCameraPosition.y << "; "
        << FrameInfo.ColorCameraPosition.z << "]"
        << std::endl;
    std::cout << "    Current Camera Position: ["
        << FrameInfo.CurrentCameraPosition.x << "; "
        << FrameInfo.CurrentCameraPosition.y << "; "
        << FrameInfo.CurrentCameraPosition.z << "]"
        << std::endl;
    std::cout << "    FilenamePath: " << FrameInfo.FilenamePath << std::endl;
    std::cout << "    HWIdentification: " << FrameInfo.HWIdentification << std::endl;
}

void PhoxiSensor::printFrameData(const pho::api::PFrame &Frame)
{
    if (Frame->Empty())
    {
        std::cout << "Frame is empty.";
        return;
    }
    std::cout << "  Frame data: " << std::endl;
    if (!Frame->PointCloud.Empty())
    {
        std::cout << "    PointCloud:    ("
            << Frame->PointCloud.Size.Width << " x "
            << Frame->PointCloud.Size.Height << ") Type: "
            << Frame->PointCloud.GetElementName()
            << std::endl;
    }
    if (!Frame->NormalMap.Empty())
    {
        std::cout << "    NormalMap:     ("
            << Frame->NormalMap.Size.Width << " x "
            << Frame->NormalMap.Size.Height << ") Type: "
            << Frame->NormalMap.GetElementName()
            << std::endl;
    }
    if (!Frame->DepthMap.Empty())
    {
        std::cout << "    DepthMap:      ("
            << Frame->DepthMap.Size.Width << " x "
            << Frame->DepthMap.Size.Height << ") Type: "
            << Frame->DepthMap.GetElementName()
            << std::endl;
    }
    if (!Frame->ConfidenceMap.Empty())
    {
        std::cout << "    ConfidenceMap: ("
            << Frame->ConfidenceMap.Size.Width << " x "
            << Frame->ConfidenceMap.Size.Height << ") Type: "
            << Frame->ConfidenceMap.GetElementName()
            << std::endl;
    }
    if (!Frame->Texture.Empty())
    {
        std::cout << "    Texture:       ("
            << Frame->Texture.Size.Width << " x "
            << Frame->Texture.Size.Height << ") Type: "
            << Frame->Texture.GetElementName()
            << std::endl;
    }
    if (!Frame->TextureRGB.Empty())
    {
        std::cout << "    TextureRGB:       ("
            << Frame->TextureRGB.Size.Width << " x "
            << Frame->TextureRGB.Size.Height << ") Type: "
            << Frame->TextureRGB.GetElementName()
            << std::endl;
    }
    if (!Frame->ColorCameraImage.Empty())
    {
        std::cout << "    ColorCameraImage:       ("
            << Frame->ColorCameraImage.Size.Width << " x "
            << Frame->ColorCameraImage.Size.Height << ") Type: "
            << Frame->ColorCameraImage.GetElementName()
            << std::endl;
    }
}