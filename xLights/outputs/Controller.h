#pragma once

#include <wx/wx.h>

#include <list>
#include <string>

#include <wx/propgrid/propgrid.h>
#include <wx/propgrid/advprops.h>

#include "Output.h"

class wxXmlNode;
class OutputManager;
class OutputModelManager;

#pragma region Controller Constants
// These are used to identify each output type
#define CONTROLLER_NULL "Null"
#define CONTROLLER_ETHERNET "Ethernet"
#define CONTROLLER_SERIAL "Serial"
#pragma endregion 

class Controller
{
protected:

#pragma region Member Variables
    OutputManager* _outputManager = nullptr; // we need this from time to time
    bool _dirty = false;                     // dirty means it needs saving
    int _id = 64001;                         // the controller id ... a unique number
    std::string _name;                       // a unique name for the controller
    std::string _description;                // a description for the controller
    bool _ok = false;                        // controller initiated ok
    bool _autoSize = false;                  // controller flexes the number of outputs to meet the needs of xLights
    bool _autoStartChannels = false;         // models on this controller can be managed by xLights
    std::list<Output*> _outputs;             // the outputs on the controller
    bool _active = true;                     // output to controller is active
    std::string _vendor;                     // the controller vendor
    std::string _model;                      // the model of the controller
    std::string _firmwareVersion;            // the firmware on the controller
    bool _suppressDuplicateFrames = false;   // should we suppress duplicate fromes
    Output::PINGSTATE _lastPingResult = Output::PINGSTATE::PING_UNKNOWN; // last ping result
#pragma endregion

public:

    #pragma region Constructors and Destructors
    Controller(OutputManager* om, wxXmlNode* node, const std::string& showDir);
    Controller(OutputManager* om);
    virtual ~Controller();
    virtual wxXmlNode* Save();
    #pragma endregion 

    #pragma region Static Functions
    // encodes/decodes string lists to indices
    static int EncodeChoices(const wxPGChoices& choices, const std::string& choice);
    static std::string DecodeChoices(const wxPGChoices& choices, int choice);

    static Controller* Create(OutputManager* om, wxXmlNode* node, std::string showDir);
    static std::list<Controller*> Discover(OutputManager* outputManager) { return std::list<Controller*>(); } // Discovers controllers supporting this connection type
    static void ConvertOldTypeToVendorModel(const std::string& old, std::string& vendor, std::string& model);
    #pragma endregion Static Functions

    #pragma region Getters and Setters
    Output* GetOutput(int outputNumber) const; // output number is zero based
    Output* GetOutput(int32_t absoluteChannel, int32_t& startChannel) const;
    std::list<Output*> GetOutputs() const { return _outputs; }
    int GetOutputCount() const { return _outputs.size(); }
    Output* GetFirstOutput() const { wxASSERT(_outputs.size() > 0); return _outputs.front(); }

    void DeleteAllOutputs();

    int32_t GetStartChannel() const;
    int32_t GetEndChannel() const;
    uint32_t GetChannels() const;

    bool IsDirty() const;
    void ClearDirty();

    const std::string& GetName() const { return _name; }
    void SetName(const std::string& name) { if (_name != name) { _name = name; _dirty = true; } }

    int GetId() const { return _id; }
    void EnsureUniqueId();

    std::string GetDescription() const { return _description; }
    void SetDescription(const std::string& description) { if (_description != description) { _description = description; _dirty = true; } }
    
    void SetAutoSize(bool autosize) { if (_autoSize != autosize) { _autoSize = autosize; _dirty = true; } }
    bool IsAutoSize() const { return _autoSize; }
    
    bool IsEnabled() const { return std::any_of(begin(_outputs), end(_outputs), [](Output* o) { return o->IsEnabled(); }); }
    void Enable(bool enable) { for (auto& it : _outputs) { it->Enable(enable); } }

    bool IsActive() const { return _active; }
    void SetActive(bool active) { if (_active != active) { _active = active;  _dirty = true; } }

    bool IsOk() const { return _ok; }

    std::string GetVendor() const { return _vendor; }
    void SetVendor(const std::string& vendor) { if (_vendor != vendor) { _vendor = vendor; _dirty = true; } }
    std::string GetModel() const { return _model; }
    void SetModel(const std::string& model) { if (_model != model) { _model = model; _dirty = true; } }
    std::string GetFirmwareVersion() const { return _firmwareVersion; }
    void SetFirmwareVersion(const std::string& firmwareVersion) { if (_firmwareVersion != firmwareVersion) { _firmwareVersion = firmwareVersion; _dirty = true; } }
    std::string GetVMF() const;

    bool IsSuppressDuplicateFrames() const { return _suppressDuplicateFrames; }
    void SetSuppressDuplicateFrames(bool suppress);

    Output::PINGSTATE GetLastPingState() const { return _lastPingResult; }
    #pragma endregion

    #pragma region Virtual Functions
    virtual void SetId(int id) { if (_id != id) { _id = id; _dirty = true; } }

    virtual void SetTransientData(int& on, int32_t& startChannel, int& nullnumber);

    virtual bool SupportsSuppressDuplicateFrames() const { return true; }
    virtual bool SupportsUpload() const { return false; }
    virtual bool IsManaged() const = 0;

    // true if this controller needs the user to be able to edit id
    virtual bool IsNeedsId() const { return true; }

    // Used on test dialog
    virtual std::string GetLongDescription() const { return GetName() + "\n" + GetDescription(); }

    // Used in xSchedule
    virtual std::string GetPingDescription() const { return GetName(); }

    // return the controller type
    virtual std::string GetType() const = 0;

    // convert an output onto this controller
    virtual void Convert(wxXmlNode* node, std::string showDir);

    // true if config needs to be rebuilt
    virtual bool NeedsControllerConfig() const { return false; }
    
    // Maximum number of outputs this controller supports ... some only support the one
    virtual int GetMaxOutputs() const { return 1; }

    // True if we can use the controller name method and model chaining on this controller type
    virtual bool IsLookedUpByControllerName() const { return false; }

    // True if this controller type can support autosize
    virtual bool SupportsAutoSize() const { return false; }

    // Used in tooltip on model dialog
    virtual std::string GetChannelMapping(int32_t ch) const = 0;
    virtual std::string GetUniverseString() const = 0;

    virtual std::string GetColumn1Label() const { return GetType(); }
    virtual std::string GetColumn2Label() const { return ""; }
    virtual std::string GetColumn3Label() const { return GetUniverseString(); }
    virtual std::string GetColumn4Label() const { return wxString::Format("%ld [%ld-%ld]", (long)GetChannels(), (long)GetStartChannel(), (long)GetEndChannel()); }
    virtual std::string GetColumn5Label() const { return GetDescription(); }
    virtual std::string GetColumn6Label() const { return wxString::Format("%d", GetId()); }

    virtual Output::PINGSTATE Ping() { _lastPingResult = Output::PINGSTATE::PING_UNAVAILABLE; return GetLastPingState(); }
    virtual void AsyncPing() { _lastPingResult = Output::PINGSTATE::PING_UNKNOWN; }
    virtual bool CanPing() const { return false; }

    virtual std::string GetSortName() const { return GetName(); }
    virtual std::string GetExport() const = 0;
    #pragma endregion 

    #pragma region Operators
    bool operator==(const Controller& controller) const { return _id == controller._id; }
    #pragma endregion 

    #pragma region UI
    #ifndef EXCLUDENETWORKUI
        virtual void AddProperties(wxPropertyGrid* propertyGrid);
	    virtual bool HandlePropertyEvent(wxPropertyGridEvent& event, OutputModelManager* outputModelManager);
        virtual void ValidateProperties(OutputManager* om, wxPropertyGrid* propGrid) const;
    #endif
    #pragma endregion
};