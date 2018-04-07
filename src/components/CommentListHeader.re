open Belt;

[%bs.raw {|require('./CommentListHeader.css')|}];

let component = ReasonReact.statelessComponent("CommentListHeader");

let make =
    (
      ~comments: option(list(CommentData.Comment.t)),
      ~totalEntries: int,
      ~totalPages: int,
      ~onPageChange: int => unit,
      ~onRejectMarked: Callback.action(unit, unit),
      ~markedForRejection: list(Model.id),
      ~showRejected: bool,
      ~onSelectAll: unit => unit,
      ~onToggleShowRejected: unit => unit,
      _children,
    ) => {
  let publishedComments = CommentData.Comment.publishedComments(comments);
  let publishedCommentsExist = List.length(publishedComments) != 0;
  let allCommentsMarkedForRejection =
    List.length(markedForRejection) != 0
    && List.length(publishedComments) == List.length(markedForRejection);
  let count =
    switch (comments) {
    | None => 0
    | Some(comments) => comments |> List.length
    };
  {
    ...component,
    render: _self =>
      <div className="CommentListHeader">
        <div className="CommentListHeader__bulkActions">
          <div className="CommentListHeader__bulkActionsSelect">
            <input
              name="markedForRejection"
              _type="checkbox"
              checked=(
                Js.Boolean.to_js_boolean(allCommentsMarkedForRejection)
              )
              disabled=(Js.Boolean.to_js_boolean(! publishedCommentsExist))
              onChange=(_event => onSelectAll())
            />
          </div>
          <AsyncButton
            onClick=onRejectMarked
            disabled=(List.length(markedForRejection) == 0)
            className="Button Button--small">
            (ReasonReact.stringToElement("Reject Marked"))
          </AsyncButton>
        </div>
        <div>
          <div className="CommentListHeader__nav">
            <div className="CommentListHeader__filters">
              <input
                name="showRejected"
                id="showRejected"
                _type="checkbox"
                checked=(Js.Boolean.to_js_boolean(showRejected))
                onChange=(_event => onToggleShowRejected())
              />
              <label
                className="CommentListHeader__filtersLabel"
                htmlFor="showRejected">
                (ReasonReact.stringToElement("Show Rejected"))
              </label>
            </div>
            <div className="CommentListHeader__count">
              (
                ReasonReact.stringToElement(
                  {j|Showing $(count) of $(totalEntries)|j},
                )
              )
            </div>
            <ReactPaginate
              pageCount=totalPages
              pageRangeDisplayed=1
              marginPagesDisplayed=1
              pageClassName="CommentListHeader__paginationItem"
              containerClassName="CommentListHeader__pagination"
              previousClassName="CommentListHeader__paginationItem"
              nextClassName="CommentListHeader__paginationItem"
              previousLinkClassName="CommentListHeader__paginationLink"
              nextLinkClassName="CommentListHeader__paginationLink"
              activeClassName="CommentListHeader__paginationItem--active"
              pageLinkClassName="CommentListHeader__paginationLink"
              breakClassName="CommentListHeader__paginationItem"
              previousLabel=<span className="fa fa-chevron-left" />
              nextLabel=<span className="fa fa-chevron-right" />
              onPageChange=(data => onPageChange(data##selected + 1))
            />
          </div>
        </div>
      </div>,
  };
};
