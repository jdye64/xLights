$(document).ready(function() {
  window.setInterval(function() {
    checkForUpdate();
  }, 1000);

  if (/Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test(navigator.userAgent)) {
    $('#sideNav').html("");
  }

});
var oldPlayingStatus = '';

function checkForUpdate() {
  console.log(playingStatus);
  console.log(oldPlayingStatus);
  if (oldPlayingStatus.status != playingStatus.status) {
    dashboardLoadStatus();
  }
  if (playingStatus.status == 'playing') {
    //update info on page
  }

  oldPlayingStatus = playingStatus;

}

function dashboardLoadStatus() {

  if (playingStatus.status == "idle") {
    //Populate Idle Page
    var notPlaying = '<p><center>Show Not Running!</center></p>';
    $('#currentStep').html(notPlaying);
    populatePlaylists();
  } else {
    //playing

    var currentPlaylist =
      `<span class="icon"><i class="icon-file"></i></span><h5>Playlist: ` +
      playingStatus.playlist + `</h5>`
    var Playing = `Song` + playingStatus.step + ` Time Left:` + playingStatus.left
      .split(".")[0] + `Duration: ` + playingStatus.length.split(".")[0];

    $("#currentStep").html(Playing);
    //Set bar
    // $("#currentPlaylistLoadingBar").css("width", findPercent(
    //   playingStatus
    //   .length.split(".")[0], playingStatus.left.split(".")[0]));
    updatePlaylistSteps(playingStatus.playlist, playingStatus.step);
  }
}

function populatePlaylists() {
  $.ajax({
    url: '/xScheduleQuery?Query=GetPlayLists&Parameters=',
    dataType: "json",
    success: function(response) {

      populatePlaylistBox("Avalible Playlists");

      $("#playlistStatus tr").remove();
      for (var i = 0; i < response.playlists.length; i++) {
        var activeclass = "";
        var notPlaying =
          `<tr>
            <td>` + response.playlists[i].name +
          `</td>
            <td>` + response.playlists[i].length.split(
            ".")[0] +
          `</td>
            <td>
              <button type="button" onclick="runCommand('Play specified playlist', '` +
          response.playlists[i].name +
          `')" class="btn btn-info btn-xs" name="button">Play</button>
            </td>
          </tr>`;

        $("#playlistStatus").append(notPlaying);
      }
    }
  });
}

function updatePlaylistSteps(playlist, currentStep) {
  $.ajax({
    url: '/xScheduleQuery?Query=GetPlayListSteps&Parameters=' + playlist,
    dataType: "json",
    success: function(response) {
      populatePlaylistBox("Current Playlist");

      $("#playlistStatus tr").remove();
      for (var i = 0; i < response.steps.length; i++) {
        var activeclass = "";
        var notPlaying =
          `<tr>
            <td>` + response.steps[i].name +
          `</td>
            <td>` + response.steps[i].length.split(
            ".")[0] +
          `</td>
            <td>
              <button type="button" onclick="runCommand('Play specified playlist', '` +
          response.steps[i].name +
          `')" class="btn btn-info btn-xs" name="button">Play</button>
              <button type="button" class="btn btn-default btn-xs" name="button">Query</button>
            </td>
          </tr>`;

        $("#playlistStatus").append(notPlaying);
      }
    }
  });
}

function populatePlaylistBox(title) {
  var html =
    `<div class="panel panel-default">
      <div class="panel-heading">
        <h3 id="currentPlaylist" class="panel-title">` +
    title +
    `</h3>
      </div>
      <div class="panel-body">
        <table class="table table-striped table-hover">
          <thead>
            <tr>
              <th>Name</th>
              <th>Duration</th>
              <th>Actions</th>
            </tr>
          </thead>
            <tbody id="playlistStatus">
          </tbody>
          </table>
      </div>
    </div>`;
  $("#homePlaylists").html(html);
}
