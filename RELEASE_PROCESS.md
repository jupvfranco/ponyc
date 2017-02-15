# How to cut a Pony release

This document is aimed at members of the Pony team who might be cutting a release of Pony. It serves as a checklist that can take you through doing a release step-by-step.

## Prerequisites for doing any release

In order to do a release, you absolutely must have:

* Commit access to the `ponyc` repo

While not strictly required, you life will be made much easier if you:

* Have a [bintray account](https://bintray.com/) and have been granted access `pony-language` organization by a "release admin".
* Have read and write access to the ponylang [travis-ci](https://travis-ci.org) account
* Have read and write access to the ponylang [appveyor](https://www.appveyor.com) account

## Prerequisites for specific releases

Before getting started, you will need a number for the version that you will be releasing as well as an agreed upon "golden commit" that will form the basis of the release.  Any commit is eligible to be a "golden commit" so long as:

* It passed all CI checks
* We were able to build "ponyc-master binaries" for it.

### Validating "ponyc-master binaries"

Currently, we validate binaries for RPM, Deb and Windows packaging. Follow the bintray links below to verify each of the package. You are looking under "Versions" for a successfully created package for the "golden commit". In the case of RPM and Deb, the versions look something like:

`0.3.1-1885.8a8ee28`

where the 

`8a8ee28`

is the sha for the commit we built from.

For windows, version look something like:

`ponyc-master-0.3.1-1526.8a8ee28`

where the 

`8a8ee28`

is the sha for the commit we built from.

* [RPM](https://bintray.com/pony-language/ponyc-rpm/ponyc-master)
* [Debian](https://bintray.com/pony-language/ponyc-debian/ponyc-master)
* [Windows](https://bintray.com/pony-language/ponyc-win/ponyc-master)

### Validate external services are functional

We rely on both Travis CI and Appveyor as part of our release process. Both need to be up and functional in order to do a release. Check the status of each before starting a release. If either is reporting issues, push the release back a day or until whenever they both are reporting no problems.

* [Travis CI Status](https://www.traviscistatus.com)
* [Appveyor Status](https://appveyor.statuspage.io)

### A GitHub issue exists for the release

All releases should have a corresponding issue in the [ponyc repo](https://github.com/ponylang/ponyc/issues). This issue should be used to communicate the progress of the release (updated as checklist steps are completed). You can also use the issue to notify other pony team members of problems.

## Releasing

For the duration of this document, we will pretend the "golden commit" version is `8a8ee28` and the version is `0.3.1`. Any place you see those values, please substitute your own version.


With that in mind, run the release script:

- bash release.sh 0.3.1 8a8ee28

### Update the GitHub issue

Leave a comment on the GitHub issue for this release to let everyone know you are done versioning the CHANGELOG and VERSION and that they are updated on `master`.

### "Update" PRs for CHANGELOG

Go through all open PRs and check to see if they are updating the CHANGELOG. If they are, leave a note asking them to merge the latest CHANGELOG into their branch because of the recent versioning for release and to move their additions into the appropriate area of "unreleased".

### Add CHANGELOG entries to GitHub releases

By now GitHub should have a listing of this new release under [releases](https://github.com/ponylang/ponyc/releases). Click the `0.3.1` then do "Edit Release". Paste the CHANGELOG entries for this release into the box and update.

### Update Homebrew

Fork the [homebrew-core repo](https://github.com/Homebrew/homebrew-core) and then clone it locally. You are going to be editing "Formula/ponyc.rb". If you already have a local copy of homebrew-core, make sure you sync up with the main Homebrew repo otherwise you might change an older version of the formula and end up with merge conflicts.

Make sure you do your changes on a branch:

* git checkout -b ponyc-0.3.1

HomeBrew has [directions](https://github.com/Homebrew/homebrew-core/blob/master/CONTRIBUTING.md#submit-a-123-version-upgrade-for-the-foo-formula) on what specifically you need to update in a formula to account for an upgrade. If you are on OSX and are unsure of how to get the SHA of the release .tar.gz, download the release file (make sure it does unzip it) and run `shasum -a 256 ponyc-0.3.1.tar.gz`. If you are on OSX, its quite possible it will try to unzip the file on your. In Safari, right clicking and selecting "Download Linked File" will get your the complete .tar.gz.

After updating the ponyc formula, push to your fork and open a PR against homebrew-core. According to the homebrew team, their preferred naming for such PRs is `ponyc 0.3.1` that is, the name of the formula being updated followed by the new version number.

### Update the GitHub issue

Leave a comment on the GitHub issue for this release letting everyone know that the Homebrew formula has been updated and a PR issued. Leave a link to your open PR.

### "Kick off" the Gentoo release

Leave a comment on the GitHub issue for this release letting @stefantalpalaru know that a release is underway. He maintains the Gentoo release. We think it involves magic, but aren't really sure.

### Wait on Travis and Appveyor

During the time since you push to the release branch, Travis CI and Appveyor have been busy building release artifacts. This can take up to a couple hours depending on how busy they are. Periodically check bintray to see if the releases are there yet.

* [RPM](https://bintray.com/pony-language/ponyc-rpm/ponyc-release)
* [Debian](https://bintray.com/pony-language/ponyc-debian/ponyc-release)
* [Windows](https://bintray.com/pony-language/ponyc-win/ponyc-release)

The pattern for releases is similar as what we previously saw. In the case of RPM and Deb, the versions look something like:

`0.3.1-1885.8a8ee28`

where the 

`8a8ee28`

is the sha for the commit we built from.

For windows, version look something like:

`ponyc-release-0.3.1-1526.8a8ee28`

where the 

`8a8ee28`

### Wait on Homebrew

Periodically check on your Homebrew PR. They have a CI process and everything should flow through smoothly. If it doesn't attempt to fix the problem. If you can't fix the problem, leave a comment on the GitHub issue for this release asking for assistance.

Note that its often quite quick to get everything through Homebrew's CI and merge process, however its often quite slow as well. We've seen their Jenkins CI often fail with errors that are unrelated to PR in question. Don't wait too long on Homebrew. If it hasn't passed CI and been merged within a couple hours move ahead without it having passed. If Homebrew is being slow about merging, when you inform IRC and pony-user of the release, note that the Homebrew version isn't available yet and include a link to the Homebrew PR and the ponyc Github release issue so that people can follow along. When the Homebrew PR is eventually merged, update pony-user and IRC.

### Update the GitHub issue as needed

At this point we are basically waiting on Travis, Appveyor and Homebrew. As each finishes, leave a note on the GitHub issue for this release letting everyone know where we stand status wise. For example: "Release 0.3.1 is now available via Homebrew"

### Inform #ponylang

Once Travis, Appveyor and Homebrew are all finished, drop a note in the #ponylang IRC channel letting everyone know that the release is out and include a link the release notes so everyone can see the CHANGELOG.

If this is an "emergency release" that is designed to get a high priority bug fix out, be sure to note that everyone is advised to update ASAP. If the high priority bug only affects certain platforms, adjust the "update ASAP" note accordingly.

### Inform pony-user

Once Travis, Appveyor and Homebrew are all finished, send an email to the pony user mailing list letting everyone know that the release is out and include a link the release notes so everyone can see the CHANGELOG.

If this is an "emergency release" that is designed to get a high priority bug fix out, be sure to note that everyone is advised to update ASAP. If the high priority bug only affects certain platforms, adjust the "update ASAP" note accordingly.

### Close the GitHub issue

Close the GitHub issue for this release
